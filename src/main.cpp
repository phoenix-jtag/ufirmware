#include <Arduino.h>
//#include <WiFi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "eeprom_interface.h"
#include "touch_interface.h"
#include "matrix_interface.h"


// WIFI - CREDENTIALS ----------------------------------------------->
//char ssid[] = "JTAG_2";    
//char pass[] = "8XCT8DFEVJ";

// FreeRTOS - QUEUE ------------------------------------------------->
QueueHandle_t touch_queue;


// FreeRTOS - TASKS ------------------------------------------------->
//void wifi_task(void *pvParameters) {

  // connect to wifi
//  WiFi.begin(ssid, pass);
//  while (WiFi.status() != WL_CONNECTED) { 
//    vTaskDelay(1000/portTICK_PERIOD_MS);
//    Serial.println("> wifi: ...");      }

//  Serial.println("> wifi: " + WiFi.localIP().toString());

//  for (;;) { // <- main loop

    //ArduinoCloud.update(); // Fix: Remove this line

//    vTaskDelay(1000/portTICK_PERIOD_MS);
 // } // <- eof main loop

//}


void touch_task(void *pvParameters) {

  // define and declaration
  touch_interface touch;
  touch_state     state;

  // define and declare vars

  for (;;) { // <- main loop

    // get touch state from touch interface
    state = touch.get_state();

    // send touch state to matrix interface
    xQueueSend(touch_queue, &state, portMAX_DELAY);

    taskYIELD(); // <- yield task
  } // <- eof main loop

}; // <- eof touch_task


void matrix_task(void *pvParameters) {

  // init base modules
  matrix_interface matrix;
  touch_state      state;
  uint8_t stat_index = 0;

  // define and declare vars
  bool power_state = false;

  for (;;) { // <- main loop

    // reveive touch state from touch_task
    if (xQueueReceive(touch_queue, &state, portMAX_DELAY)) {

      // where we will recive signals from different channels
      // and they will define the state of the matrix
      switch (state) { 

        case touch_state::TOUCH1:
          if (power_state) { // only
            matrix.set_state(static_cast<matrix_state>(stat_index++));
          }
          break;

        case touch_state::TOUCH2:
          break;

        case touch_state::TOUCH3:
          break;

        case touch_state::PRESS:
          if (power_state == 0) {
            matrix.set_state(matrix_state::BLACK);
            //matrix.display();
            //matrix.set_state(matrix_state::BLACK);
          } else {
            matrix.set_state(matrix_state::PRIDE);
            //matrix.display();
            //matrix.set_state(matrix_state::PRIDE);
          }
            
          power_state = !power_state;
          break;

        case touch_state::HOLD:
          break;

        default:
          break;
      } // eof switch

    } // eof touch state

    matrix.display(); // <- display matrix state

    taskYIELD(); // <- yield task
  } // <- eof main loop

}; // <- eof matrix_task


void setup() {
  // BASIC - INITIALIZATION ----------------------------------------->
  Serial.begin(115200); delay(50);   
  eeprom_interface eeprom;                    // <- init eeprom memory                              

  // FRTOS - tasks and queues --------------------------------------->
  touch_queue = xQueueCreate(8, sizeof(touch_state));
  //xTaskCreate(wifi_task,   "wifi_task",   4096, NULL, 1, NULL);
  xTaskCreate(touch_task,  "touch_task",  4096, NULL, 1, NULL);
  xTaskCreate(matrix_task, "matrix_task", 4096, NULL, 1, NULL);

  // FRTOS - start scheduler ---------------------------------------->
  vTaskStartScheduler();
}






// DEPRECATED
void loop() {}