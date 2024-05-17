#include <Arduino.h>
#include "touch_interface.h"

int touch_interface::touch_count = 0;
volatile unsigned long touch_interface::last_interrupt_time = 0;
volatile unsigned long touch_interface::last_touch_time = 0;

void touch_interface::touch_isr() {
    unsigned long interrupt_time = millis(); // get current time

    // If interrupts come faster than TOUCH_TIME, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > TOUCH_TIME) {
        last_interrupt_time = interrupt_time; // update last interrupt time
        touch_count++;
        last_touch_time = interrupt_time;
    }
}


touch_interface::touch_interface() {
  touch_count = 0;
  last_interrupt_time = 0;
  last_touch_time = 0;

  touchAttachInterrupt(TOUCH_PIN, touch_isr, THRESHOLD);
}

touch_interface::~touch_interface() {
  touchDetachInterrupt(TOUCH_PIN);
}


void touch_interface::monitor() {
  if (touch_count > 0 && millis() - last_touch_time > EXPECTATION_TIME) {
     switch (touch_count) {
      case 1:
        Serial.println("> 1x click");
        break;
      case 2:
        Serial.println("> 2x click");
        break;
      case 3:
        Serial.println("> 3x click");
        break;
      default:
        Serial.println("> pressed");
        break;
    }
    touch_count = 0;
  }
}