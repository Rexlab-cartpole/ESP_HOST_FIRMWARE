#include <Arduino.h>

extern void encoder_setup();

extern void serial_task(void *pvParameters);
extern void motor_task(void *pvParameters);
extern void encoder_task(void *pvParameters);

void setup() {
  // Serial.begin(115200);
  Serial.begin(38400);

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


