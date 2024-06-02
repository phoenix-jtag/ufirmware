#include "task_manager.h"


task_manager::task_manager() {
    touch_queue = xQueueCreate(10, sizeof(touch_state));
}


task_manager::~task_manager() {
    vQueueDelete(touch_queue);
}



void task_manager::start_tasks() {
    xTaskCreate((TaskFunction_t)&task_manager::uart_task, "uart_task", 4096, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)&task_manager::touch_task, "touch_task", 4096, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)&task_manager::matrix_task, "matrix_task", 4096, NULL, 1, NULL);
}




void task_manager::uart_task(void *pvParameters) {

  String command = "";

  for (;;) {

    while (Serial.available() > 0) {

      char cmd = Serial.read();

      switch(cmd) {
        case '1': Serial.println("> touch_cal: touch sensor"); break;

        default: Serial.println("> unknown cmd "); break;
      }

    } // <- eof while
  } // <- eof main loop
} // <- eof uart_task



void task_manager::touch_task(void *pvParameters) {

  for (;;) { // <- main loop

    // get touch state from touch interface
    state = touch.get_state();

    // send touch state to matrix interface
    xQueueSend(touch_queue, &state, portMAX_DELAY);

    taskYIELD(); // <- yield 
    
  } // <- eof main loop
}; // <- eof touch_task



void task_manager::matrix_task(void *pvParameters) {

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

        default:
          break;

      } // eof switch
    } // eof touch state

    matrix.display(); // <- display matrix state

    taskYIELD(); // <- yield task

  } // <- eof main loop
}; // <- eof matrix_task