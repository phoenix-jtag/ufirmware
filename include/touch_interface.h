#pragma once

//#define DEBUG 1

#define TOUCH_PIN T0 
#define THRESHOLD 54

#define TOUCH_THRESHOLD  150     // touch registered after 150ms (50 - 200 ms)
#define TIMEOUT          500     // start processing after 500ms

#define PRESS_THRESHOLD   1000   // press registered after 1.5s
#define HOLD_THRESHOLD    5000   // hold registered after 5s
#define RESET_THRESHOLD   10000  // reset registered after 10s


enum class touch_state {
    UNDEF,
    IDLE,
    TOUCH1,
    TOUCH2,
    TOUCH3,
    DETECT,
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