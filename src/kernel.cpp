#include "kernel.h"

// STRUCT DEFINITIONS
kernel_states kernel::kernel_state;
kernel_config kernel::kernel_conf;

#define POWER_OFF matrix_states::POWER_OFF
#define POWER_ON  matrix_states::POWER_ON


kernel::kernel() {

	eeprom_api& eeprom 	= eeprom_api::getInstance();

	// GET DINAMIC CONFIGURATION
	eeprom.get_page(pages_list::KERNEL, sizeof(kernel_conf), &kernel_conf);

	// CHK DINAMIC CONFIGURATION
	if (kernel_conf.state != MAGIC) { // VALIDATION FAILED

		// APPLY DEFAULT CONFIG
		kernel_conf.state = MAGIC;

		// SAVE DEFAULT CONFIG
		eeprom.set_page(pages_list::KERNEL, sizeof(kernel_conf), &kernel_conf);
	}

	kernel_state = kernel_states::INITED;
}


kernel::~kernel() {

	eeprom_api& eeprom 	= eeprom_api::getInstance();
			
	// SAVE CONFIG STRUCT to EEPROM_CONFIG
	eeprom.set_page(pages_list::KERNEL, sizeof(kernel_conf), &kernel_conf);
}


void kernel::start() {

	// CREATE TASKS
	xTaskCreate(kernel::uart_task, "uart_task", 2048, this, 1, NULL);
	//xTaskCreate(kernel::touch_task, "touch_task", 2048, this, 1, NULL);
	xTaskCreate(kernel::matrix_task, "matrix_task", 2048, this, 1, NULL);

	kernel_state = kernel_states::EXECUTION;
}



void kernel::uart_task(void *pvParameters) {

	touch_api&  touch	= touch_api ::getInstance();

	String command = "";

	for (;;) {

		while (Serial.available() > 0) {

			char cmd = Serial.read();

			switch(cmd) {
				case '1': 
				if (touch.touch_calibration()) {
					Serial.println("> touch calibration saving - success");
				} else {
					Serial.println("> touch calibration saving - failed");
				}
				
				
				break;

				default:  Serial.println("> unknown cmd "); break;
			}

		} // <- eof while
	} // <- eof main loop
} // <- eof uart_task



void kernel::touch_task(void *pvParameters) {

	//kernel* instance = static_cast<kernel*>(pvParameters);
	//eeprom_api& eeprom 	= eeprom_api::getInstance();
	//touch_api&  touch	= touch_api ::getInstance();
	//matrix_api& matrix	= matrix_api::getInstance();

	// definitions and declarations

	for (;;) { // <- main loop

		// get touch state from touch interface
		//touch_state = touch.get_state();

		// send touch state to matrix interface
		//xQueueSend(touch_queue, &touch_state, portMAX_DELAY);

		taskYIELD(); // <- yield 
		
	} // <- eof main loop
}; // <- eof touch_task



void kernel::matrix_task(void *pvParameters) {

	//kernel* instance = static_cast<kernel*>(pvParameters);
	//eeprom_api& eeprom 	= eeprom_api::getInstance();
	touch_api&  touch	= touch_api ::getInstance();
	matrix_api& matrix	= matrix_api::getInstance();

	// definitions and declarations
	touch_states  touch_state  = touch_states::IDLE;
	matrix_states matrix_state = POWER_OFF;

	TaskHandle_t prideTaskHandle = NULL;
	TaskHandle_t blackTaskHandle = NULL;

	
	vTaskSuspend(prideTaskHandle);

	xTaskCreate(matrix.black_task, "black_task", 2048, NULL, 1, &blackTaskHandle);
	vTaskSuspend(blackTaskHandle);

	for (;;) { // <- MAIN LOOP

		// get touch state from touch api
		touch_state = touch.get_state();

		// handle touch state
		if (touch_state != touch_states::IDLE) {

			// handle touch state
			switch (touch_state) { 

				case touch_states::TOUCH1:
					break;

				case touch_states::TOUCH2:
					break;

				case touch_states::TOUCH3:
					break;

				case touch_states::PRESS:
    				if (matrix_state == POWER_OFF) {
        				matrix_state = POWER_ON;
						vTaskSuspend(blackTaskHandle);
						vTaskResume(prideTaskHandle);
    				} else {
        				matrix_state = POWER_OFF;
        				vTaskSuspend(prideTaskHandle);
						vTaskResume(blackTaskHandle);
    				}
    				break;

				default:
					break;

			} // eof switch
		} // eof touch state

		taskYIELD(); // <- yield task

	} // <- eof main loop
}; // <- eof matrix_task