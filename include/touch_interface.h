#pragma once

#define TOUCH_PIN T0 
#define THRESHOLD 56

#define TOUCH_THRESHOLD  150   // debounce time in milliseconds 50-200
#define TIMEOUT          500    // 350ms to register next click to separate clicks

#define PRESS_THRESHOLD   1500   // 3s to register long press
#define HOLD_THRESHOLD    5000   // 1s to register hold
#define RESET_THRESHOLD   10000  // 10s to reset to default config


enum class touch_state {
    UNDEF,
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
    touch_state state;

    static int touch_count;
    static volatile unsigned long touch_start_time;
    static volatile unsigned long last_touch_time;
    static volatile unsigned long last_interrupt_time;


    static void touch_isr();

public:
    
    touch_interface();
    ~touch_interface();

    touch_state get_state();
};