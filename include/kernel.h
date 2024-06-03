#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "eeprom_api.h"
#include "touch_api.h"
#include "matrix_api.h"


enum class kernel_states {

    NO_INITED,  // ram structures != eeprom structures
    IS_INITED,  // ram structures == eeprom structures
    EXECUTION,  // kernel is running, all tasks are active
    FAILURE,    // kernel is in failure state
};


struct kernel_config {

    bool is_default;
};



class kernel { 

private: 
    // kernel partition
    kernel_states   kernel_state;
    kernel_config   kernel_conf;

    // eeprom partition
    eeprom_api      eeprom;
    eeprom_states   eeprom_state;
    eeprom_config   eeprom_conf;    

    // touch  partition
    touch_api       touch;
    touch_states    touch_state;
    touch_config    touch_conf;

    QueueHandle_t touch_queue;

    // matrix partition
    matrix_api      matrix;
    matrix_states   matrix_state;
    matrix_config   matrix_conf;

    uint8_t         stat_index = 0;
    bool            power_state = false;


public: 

    kernel();
    ~kernel();

    bool init();
    bool kill();

    void tasks_start();
    void tasks_stop();
    void tasks_monitor();

    void uart_task(void *pvParameters);
    void touch_task(void *pvParameters);
    void matrix_task(void *pvParameters);

}; // <- eof task_manager