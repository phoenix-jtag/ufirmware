#pragma once
#include <Button2.h>

// #define DEBUG 1

#define TOUCH_PIN T0 
#define TOUCH_THRESHOLD 70
#define DEBOUNCE_TIME   65


enum class touch_state {

    IDLE,
    TOUCH1,
    TOUCH2,
    TOUCH3,
    PRESS,
    HOLD,
    RESET,
};


class touch_interface {

private:

    static touch_state state;

    Button2 button;

    static void got_touch_event();
    static byte cap_state_handler();

    static void touch1_handler(Button2 &btn) {
        Serial.println("> 1 touch");
        state = touch_state::TOUCH1;
    }

    static void touch2_handler(Button2 &btn) {
        Serial.println("> 2 touch");
        state = touch_state::TOUCH2;
    }

    static void touch3_handler(Button2 &btn) {
        Serial.println("> 3 touch");
        state = touch_state::TOUCH3;
    }

    static void press_handler(Button2 &btn) {
        Serial.println("> press");
        state = touch_state::PRESS;
    }

    static void hold_handler(Button2 &btn) {
        Serial.println("> hold");
        state = touch_state::HOLD;
    }

public:
    
    touch_interface();
    ~touch_interface();

    touch_state get_state();
};