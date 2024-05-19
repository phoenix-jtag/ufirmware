#include <Arduino.h>

#include "eeprom_interface.h"
#include "touch_interface.h"
#include "matrix_interface.h"


void setup() {
  // ----------------------------------------------------------------> INIT UART
  Serial.begin(115200); delay(50);                                  // <- wait for serial monitor

  // ----------------------------------------------------------------> INIT EEPROM
  eeprom_interface eeprom;                                            // <- load default or custom config

  //-----------------------------------------------------------------> INIT TOUCH
  touch_interface  touch;                                             // <- touch interrupt setup

  // ----------------------------------------------------------------> INIT MATRIX
  matrix_interface matrix;                                            // <- matrix setup

  // ----------------------------------------------------------------> some variables
  bool power_state = false;

  // FRTOS - SHEDDULER EMULATION ------------------------------------> 

  for (;;) {
    switch (touch.get_state()) {
      case touch_state::TOUCH1:
        break;
      case touch_state::TOUCH2:
        break;
      case touch_state::TOUCH3:
        break;
      case touch_state::PRESS:
        power_state = !power_state;
      default:
        break;
    }

    power_state == LOW ? matrix.display(matrix_state::BLACK) : matrix.display(matrix_state::PRIDE);
  }
}

// DEPRECATED
void loop() {}