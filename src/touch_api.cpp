#include "touch_api.h"

// STRUCT DEFINITIONS
touch_states touch_api::touch_state;
touch_config touch_api::touch_conf;

// VARIABLES DEFINITIONS
bool	touch_active;
bool	last_touch_active;
bool	testing_lower;
byte	button_state;



touch_api::touch_api() {

	// GET DINAMIC CONFIGURATION
	eeprom.get_page(pages_list::TOUCH, sizeof(touch_conf), &touch_conf);

	// CHK DINAMIC CONFIGURATION
	if (touch_conf.state != MAGIC) { // VALIDATION FAILED

		// APPLY DEFAULT CONFIG
		touch_conf.state			= MAGIC;
		touch_conf.threshold_lvl	= DEFAULT_THRESHOLD_LVL;
		touch_conf.debounce_time	= DEFAULT_DEBOUNCE_TIME;

		// SAVE DEFAULT CONFIG
		eeprom.set_page(pages_list::TOUCH, sizeof(touch_conf), &touch_conf);
	} 

	// SET STATIC CONFIGURATION
	touch_active		= false;
	last_touch_active	= false;
	testing_lower		= true; 
	button_state		= HIGH;

	// INIT FUNCTIONALITY
	touchAttachInterrupt(TOUCH_PIN, got_touch_event, touch_conf.threshold_lvl);
	touchInterruptSetThresholdDirection(testing_lower);

	button.setDebounceTime(touch_conf.debounce_time);
	button.setButtonStateFunction(cap_state_handler);
	button.setClickHandler(touch1_handler);
	button.setDoubleClickHandler(touch2_handler);
	button.setTripleClickHandler(touch3_handler);
	button.setLongClickHandler(press_handler);

	button.begin(BTN_VIRTUAL_PIN);

	// display touch configuration
	Serial.println("> touch: L[" + String(touch_conf.threshold_lvl) + "] T[" + String(touch_conf.debounce_time) + "]");
}



touch_api::~touch_api() {
	
	touchDetachInterrupt(TOUCH_PIN);
}



void touch_api::got_touch_event(){

	if (last_touch_active != testing_lower) {

	touch_active = !touch_active;
	testing_lower = !testing_lower;
	// Touch ISR will be inverted: Lower <--> Higher than the Threshold after ISR event is noticed
	touchInterruptSetThresholdDirection(testing_lower);
	}
}



byte touch_api::cap_state_handler() {

	return button_state;
}



touch_states touch_api::get_state() {

	#ifdef DEBUG
	Serial.println("Capacity: " + String(touchRead(TOUCH_PIN)));
	#endif

	touch_state = touch_states::IDLE;

	button.loop();

	if(last_touch_active != touch_active){

		last_touch_active = touch_active;

		if (touch_active) {
			button_state = LOW;
		} else {
			button_state = HIGH;
		}
	}
	
	return touch_state;
}



bool touch_api::touch_calibration() {

	uint8_t new_threshold = 0; 	// <- new threshold value

	// current touch threshold
	Serial.println();
	Serial.println("> TOUCH_CALIBRATION");
	Serial.println("> touch_threshold: " + String(touch_conf.threshold_lvl));
	Serial.println();

	// measure touch threshold
	for (uint8_t i = 0; i < 3; i++) { // 3 samples

		Serial.println("> press touch: " + String(i + 1));

		button.waitForLong();
		new_threshold += touchRead(TOUCH_PIN);
	}

	// calculate new threshold
	new_threshold = (new_threshold / 3);

	// display new threshold
	Serial.println();
	Serial.println("> TEST NEW_THRESHOLD: " + String(new_threshold));
	Serial.println();

	// aplly new threshold
	touchDetachInterrupt(TOUCH_PIN);
	touchAttachInterrupt(TOUCH_PIN, got_touch_event, new_threshold);

	// test new threshold
	for (uint8_t i = 0; i < 3; i++) { // 3 samples

		Serial.println("> press touch: " + String(i + 1));

		button.waitForLong();
		Serial.println("> ----> " + String(touchRead(TOUCH_PIN)));
	}

	// ask user to save new threshold
	Serial.println();
	Serial.println("> SAVE NEW THRESHOLD == " + String(new_threshold) + " ? [y/n]: ");

	while (Serial.available() == 0) {}
	char answer = Serial.read();
	if (answer == 'y') { 
		
		// save new threshold
		touch_conf.threshold_lvl = new_threshold;
		return eeprom.set_page(pages_list::TOUCH, sizeof(touch_conf), &touch_conf);


	} else { 

		// restore old threshold
		touchDetachInterrupt(TOUCH_PIN);
		touchAttachInterrupt(TOUCH_PIN, got_touch_event, touch_conf.threshold_lvl);
		return false;
	} 
}