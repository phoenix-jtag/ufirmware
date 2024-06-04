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

	uint8_t     state;
};



class kernel { 

private: 
	// kernel objects
	static kernel_states   kernel_state;
	static kernel_config   kernel_conf;

	kernel();
	~kernel();

	static void uart_task(void *pvParameters);
	static void touch_task(void *pvParameters);
	static void matrix_task(void *pvParameters);

public: 
	// singleton pattern
    static kernel& getInstance() {
        static kernel instance; // Guaranteed to be destroyed.
        return instance;            // Instantiated on first use.
    }
    kernel(kernel const&) = delete; // Deleting the copy constructor
    void operator=(kernel const&) = delete; // Deleting the assignment operator

	

	//bool init();
	//bool kill();

	void start();
	//void tasks_stop();
	//void tasks_monitor();

	

}; // <- eof task_manager