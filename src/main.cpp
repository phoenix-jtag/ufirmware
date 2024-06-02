#include <Arduino.h>
#include "task_manager.h"



void setup() {

  // BASIC - INITIALIZATION -----------------------------------------> 
  Serial.begin(115200); delay(50);  
  eeprom_interface eeprom;                    // <- init eeprom memory 

  // FRTOS - INITIALIZATION ----------------------------------------->  
  task_manager task_manager;                  // <- init task manager  
  task_manager.start_tasks();                 // <- start tasks                           

  // FRTOS - start scheduler ---------------------------------------->
  vTaskStartScheduler();
}






// DEPRECATED
void loop() {}