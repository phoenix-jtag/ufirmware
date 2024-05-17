#include <Arduino.h>
#include "presets.h"
#include "eeprom_interface.h"
#include "touch_interface.h"


void setup() {
  // ----------------------------------------------------------------> INIT UART
  Serial.begin(115200); delay(50);                                  // <- wait for serial monitor

  // ----------------------------------------------------------------> INIT EEPROM
  eeprom_interface eeprom;                                            // <- load default or custom config
  Serial.print("> device_id: ");
  Serial.println(eeprom.config.device_id);

  //-----------------------------------------------------------------> INIT TOUCH
  touch_interface  touch;                                             // <- touch interrupt setup



  // FRTOS - SHEDDULER EMULATION ------------------------------------> 
 for(;;) {

    touch.monitor();

    pride();
    FastLED.show();
  }
}

// DEPRECATED
void loop() {}