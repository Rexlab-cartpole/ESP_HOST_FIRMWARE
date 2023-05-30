/*

Ideas: 
* Run motor task in core 0 since main loop is on core 1
* use biuffering to avoid serial prints: https://www.forward.com.au/pfod/ArduinoProgramming/RealTimeArduino/index.html
* Profile each of the tasks using the above
* use fast accel
* use a semaphore for serial-motor command updates
* try moving encoder task to rtos with watchdogs disabled

*/


#include <Arduino.h>

extern void encoder_setup();

extern void serial_task(void *pvParameters);
extern void motor_task(void *pvParameters);
extern void encoder_task(void *pvParameters);

void setup() {
  // Serial.begin(115200);
  // Serial.begin(38400);
  Serial.begin(256000);

  encoder_setup();

  /*
  Tasks: 

  * Serial communication
  * Motor control
  * Sensor reading
  * Screen display
  */

  // xTaskCreatePinnedToCore(
  //   serial_task,
  //   "Serial Task",
  //   1000,
  //   NULL,
  //   1,
  //   NULL,
  //   0
  // );

  xTaskCreatePinnedToCore(
    motor_task,
    "Motor Task",
    1000,
    NULL,
    1,
    NULL,
    1
  );

  // xTaskCreate(
  //   motor_task,
  //   "Motor Task",
  //   1000,
  //   NULL,
  //   1,
  //   NULL
  // );

  // xTaskCreate(
  //   encoder_task,
  //   "Encoder Task",
  //   1000,
  //   NULL,
  //   1,
  //   NULL
  // );
}

void loop() {
  encoder_task(NULL);
  serial_task(NULL);
}


