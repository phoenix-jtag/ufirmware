#pragma once
#include <Arduino.h>
#include <Button2.h>
#include "eeprom_api.h"

// #define DEBUG 1

#define TOUCH_PIN T0 
#define TOUCH_THRESHOLD 70
#define DEBOUNCE_TIME   65


enum class touch_states {

    IDLE,
    TOUCH1,
    TOUCH2,
    TOUCH3,
    PRESS,
    HOLD,
    RESET,
};


struct touch_config {

    uint8_t 
    uint8_t touch_threshold;
    uint8_t debounce_time;
};



class touch_api {

private:

    static touch_states touch_state;
    static touch_config touch_conf;

    Button2 button;

    eeprom_api& eeprom = eeprom_api::getInstance();

    static void got_touch_event();
    static byte cap_state_handler();

    static void touch1_handler(Button2 &btn) {
        Serial.println("> 1 touch");
        touch_state = touch_states::TOUCH1;
    }

    static void touch2_handler(Button2 &btn) {
        Serial.println("> 2 touch");
        touch_state = touch_states::TOUCH2;
    }

    static void touch3_handler(Button2 &btn) {
        Serial.println("> 3 touch");
        touch_state = touch_states::TOUCH3;
    }

    static void press_handler(Button2 &btn) {
        Serial.println("> press");
        touch_state = touch_states::PRESS;
    }

    static void hold_handler(Button2 &btn) {
        Serial.println("> hold");
        touch_state = touch_states::HOLD;
    }

public:
    
    touch_api();
    ~touch_api();

    touch_states get_state();
};