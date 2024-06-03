#pragma once
#include <Button2.h>

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

};



class touch_api {

private:

    static touch_states  state;
    static touch_config config;

    Button2 button;

    static void got_touch_event();
    static byte cap_state_handler();

    static void touch1_handler(Button2 &btn) {
        Serial.println("> 1 touch");
        state = touch_states::TOUCH1;
    }

    static void touch2_handler(Button2 &btn) {
        Serial.println("> 2 touch");
        state = touch_states::TOUCH2;
    }

    static void touch3_handler(Button2 &btn) {
        Serial.println("> 3 touch");
        state = touch_states::TOUCH3;
    }

    static void press_handler(Button2 &btn) {
        Serial.println("> press");
        state = touch_states::PRESS;
    }

    static void hold_handler(Button2 &btn) {
        Serial.println("> hold");
        state = touch_states::HOLD;
    }

public:
    
    touch_api();
    ~touch_api();

    touch_states get_state();
};