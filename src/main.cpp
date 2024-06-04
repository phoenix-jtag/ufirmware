#include <Arduino.h>

//#define DEBUG 1
#include "kernel.h"


void setup() {

	// BASIC - INITIALIZATION -----------------------------------------> 
	Serial.begin(115200); delay(100);       // <- init serial port
	//eeprom_api eeprom;                    // <- init eeprom memory 

	// KERNEL - INITIALIZATION ---------------------------------------->  
	kernel& kernel = kernel::getInstance(); // <- get kernel instance
    kernel.start();                       // <- init kernel memory                 

	// FRTOS - start scheduler ---------------------------------------->
	//vTaskStartScheduler();
}






// DEPRECATED
void loop() {}