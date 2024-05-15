#include <Arduino.h>
#include "presets.h"
#include "eeprom_manager.h"
#include "epages_manager.h"


void setup() {
  eeprom_manager eeprom;
  epages_manager epages;
}


void loop()
{
  pride();
  FastLED.show();  
}


