#include "kernel.h"


kernel::kernel() {

  kernel_state = kernel_states::NO_INITED;

    touch_queue = xQueueCreate(10, sizeof(touch_states));
}


kernel::~kernel() {
    vQueueDelete(touch_queue);
}





void kernel::tasks_start() {
    xTaskCreate((TaskFunction_t)&kernel::uart_task, "uart_task", 4096, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)&kernel::touch_task, "touch_task", 4096, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)&kernel::matrix_task, "matrix_task", 4096, NULL, 1, NULL);
}




void kernel::uart_task(void *pvParameters) {

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



void kernel::touch_task(void *pvParameters) {

  for (;;) { // <- main loop

    // get touch state from touch interface
    state = touch.get_state();

    // send touch state to matrix interface
    xQueueSend(touch_queue, &state, portMAX_DELAY);

    taskYIELD(); // <- yield 
    
  } // <- eof main loop
}; // <- eof touch_task



void kernel::matrix_task(void *pvParameters) {

  for (;;) { // <- main loop

    // reveive touch state from touch_task
    if (xQueueReceive(touch_queue, &state, portMAX_DELAY)) {

      // where we will recive signals from different channels
      // and they will define the state of the matrix
      switch (state) { 

        case touch_states::TOUCH1:
          if (power_state) { // only
            matrix.set_state(static_cast<matrix_states>(stat_index++));
          }
          break;

        case touch_states::TOUCH2:
          break;

        case touch_states::TOUCH3:
          break;

        case touch_states::PRESS:
          if (power_state == 0) {
            matrix.set_state(matrix_states::BLACK);
            //matrix.display();
            //matrix.set_state(matrix_states::BLACK);
          } else {
            matrix.set_state(matrix_states::PRIDE);
            //matrix.display();
            //matrix.set_state(matrix_states::PRIDE);
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