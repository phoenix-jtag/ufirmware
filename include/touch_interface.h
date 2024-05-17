#pragma once

#define TOUCH_PIN T0 
#define THRESHOLD 56

#define TOUCH_TIME 150 // debounce time in milliseconds 50-200
#define EXPECTATION_TIME 500   // 350ms to register next click to separate clicks



class touch_interface {
private:
    static int touch_count;
    static volatile unsigned long last_interrupt_time;
    static volatile unsigned long last_touch_time;

    static void touch_isr();

public:
    
    touch_interface();
    ~touch_interface();

    void monitor();
};