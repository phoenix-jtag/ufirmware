#include <Arduino.h>
#include "touch_interface.h"



touch_state touch_interface::state;

uint8_t  touch_threshold;
uint8_t  debounce_time;
bool     touch_active;
bool     last_touch_active;
bool testing_lower;
byte button_state;



void touch_interface::got_touch_event(){

  if (last_touch_active != testing_lower) {

    touch_active = !touch_active;
    testing_lower = !testing_lower;
    // Touch ISR will be inverted: Lower <--> Higher than the Threshold after ISR event is noticed
    touchInterruptSetThresholdDirection(testing_lower);
  }
}



byte touch_interface::cap_state_handler() {
    return button_state;
}



touch_interface::touch_interface() {

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

touch_interface::~touch_interface() {
  touchDetachInterrupt(TOUCH_PIN);
}



touch_state touch_interface::get_state() {

  #ifdef DEBUG
    Serial.println("Capacity: " + String(touchRead(TOUCH_PIN)));
  #endif

  state = touch_state::IDLE;

  button.loop();

  if(last_touch_active != touch_active){

    last_touch_active = touch_active;

    if (touch_active) {
      button_state = LOW;
    } else {
      button_state = HIGH;
    }
  }
  
  return state;
}