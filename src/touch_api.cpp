#include "touch_api.h"

touch_states touch_api::touch_state;
touch_config touch_api::touch_conf;

// STATIC VARIABLES
bool     touch_active;
bool     last_touch_active;
bool     testing_lower;
byte     button_state;



touch_api::touch_api() {

  // LOAD TOUCH CONFIGURATION
  eeprom.get_page(pages_list::TOUCH, sizeof(touch_conf), &touch_conf);

  // VERIFY TOUCH CONFIGURATION
  


  touch_threshold    = TOUCH_THRESHOLD;   
  debounce_time      = DEBOUNCE_TIME; 

  touch_active       = false;
  last_touch_active  = false;
  testing_lower      = true; 
  button_state       = HIGH;

  touchAttachInterrupt(TOUCH_PIN, got_touch_event, touch_threshold);

  // Touch ISR will be activated when touchRead is lower than the Threshold
  touchInterruptSetThresholdDirection(testing_lower);

    button.setDebounceTime(debounce_time);

    button.setButtonStateFunction(cap_state_handler); // Fixed method name
    button.setClickHandler(touch1_handler);
    button.setDoubleClickHandler(touch2_handler);
    button.setTripleClickHandler(touch3_handler);

    button.setLongClickHandler(press_handler);

    button.begin(BTN_VIRTUAL_PIN);
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