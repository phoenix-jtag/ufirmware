#include <Arduino.h>
#include "touch_interface.h"

int touch_interface::touch_count = 0;
volatile unsigned long touch_interface::touch_start_time = 0;
volatile unsigned long touch_interface::last_touch_time = 0;
volatile unsigned long touch_interface::last_interrupt_time = 0;

void touch_interface::touch_isr() {
    unsigned long interrupt_time = millis(); // get current time

    #ifdef DEBUG
    Serial.println("> debug: touch_isr - " + String(touchRead(TOUCH_PIN)));
    #endif

    // If interrupts come faster than TOUCH_TIME, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > TOUCH_THRESHOLD) {
      last_interrupt_time = interrupt_time; // update last interrupt time
      touch_count++;
      last_touch_time = interrupt_time;
    }

    if (touch_count == 1) { // if this is the first touch
      touch_start_time = millis(); // record the start time
    }
}


touch_interface::touch_interface() {
  touch_count = 0;

  touch_start_time = 0;
  last_touch_time = 0;
  last_interrupt_time = 0;

  state = touch_state::IDLE;

  touchAttachInterrupt(TOUCH_PIN, touch_isr, THRESHOLD);
}

touch_interface::~touch_interface() {
  touchDetachInterrupt(TOUCH_PIN);
}


touch_state touch_interface::get_state() {

  state = touch_state::IDLE;
  //Serial.println("> touch_count: " + String(touch_count));

  if (touch_count > 0 && millis() - last_touch_time > TIMEOUT) {
    unsigned long touch_duration = millis() - touch_start_time;

    if (touch_duration > RESET_THRESHOLD) { 

      Serial.println("> reset");
      state = touch_state::RESET;

    } else if (touch_duration > HOLD_THRESHOLD) { 

      Serial.println("> hold");
      state = touch_state::HOLD;

    } else if (touch_duration > PRESS_THRESHOLD) {

      Serial.println("> press");
      state = touch_state::PRESS;

    } else if (touch_duration <= PRESS_THRESHOLD) { 

    switch (touch_count) {
      case 1:
            Serial.println("> 1x click");
            state = touch_state::TOUCH1;
            break;
      case 2:
            Serial.println("> 2x click");
            state = touch_state::TOUCH2;
            break;
      case 3:
            Serial.println("> 3x click");
            state = touch_state::TOUCH3;
            break;
      default:
            Serial.println("> undef");
            state = touch_state::UNDEF;
            break;
      }
    }
      touch_count = 0;
      touch_start_time = millis(); // reset touch start time
  }
  
  return state;
}