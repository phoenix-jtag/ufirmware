#include <Arduino.h>

//#define DEBUG 1
#include "kernel.h"


void setup() {

	// BASIC - INITIALIZATION -----------------------------------------> 
	Serial.begin(115200); delay(100);       // <- init serial port

	// KERNEL - INITIALIZATION ---------------------------------------->  
	kernel& kernel = kernel::getInstance(); // <- get kernel instance
    kernel.start();                       	// <- init kernel memory                 
}






// DEPRECATED
void loop() {}