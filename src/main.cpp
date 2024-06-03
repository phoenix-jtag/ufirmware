#include <Arduino.h>
#include "kernel.h"



void setup() {

  // BASIC - INITIALIZATION -----------------------------------------> 
  Serial.begin(115200); delay(50);  
  eeprom_api eeprom;                    // <- init eeprom memory 

  // KERNEL - INITIALIZATION ---------------------------------------->
  kernel instance;                        // <- init kernel memory                       

  // FRTOS - start scheduler ---------------------------------------->
  vTaskStartScheduler();
}






// DEPRECATED
void loop() {}