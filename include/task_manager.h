#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "eeprom_interface.h"
#include "touch_interface.h"
#include "matrix_interface.h"

class task_manager { 

private: 
    QueueHandle_t touch_queue;

    // touch_task
    touch_interface touch;
    touch_state     state;

    // matrix_task
    matrix_interface matrix;
    touch_state      state;
    uint8_t          stat_index = 0;
    bool             power_state = false;

public: 

    task_manager();
    ~task_manager();

    void start_tasks();

    void uart_task(void *pvParameters);
    void touch_task(void *pvParameters);
    void matrix_task(void *pvParameters);

}; // <- eof task_manager