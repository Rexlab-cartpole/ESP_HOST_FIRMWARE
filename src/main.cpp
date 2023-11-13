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
#include "common_types.h"

// MOTOR CONTROL
extern void setMotorTorque(computer_commands_t commands);
extern computer_commands_t safety_check(computer_commands_t commands, encoderPositions_t positions);
extern void motorSetup();
extern void homingRoutine();
extern void enableMotors();
extern void disableMotors();

// ENCODER READING
extern void encoderSetup();
extern encoderPositions_t getEncoderPositionsScaled();

// SERIAL
extern computer_commands_t serial_getLatestComputerCommands();
extern void serial_sendEncoderPositions(encoderPositions_t position);

controllerState_e controllerState;

void setup () { 
  controllerState = controllerState_e::INIT;

  Serial.begin(115200);
  // while (!Serial) {;}
  Serial.println("Controller Alive");

#ifdef VESC_DEBUG
  #ifdef VESC_DEBUG_LINEAR
    Serial1.begin(115200, SERIAL_8N1, LINEAR_MOTOR_SERIAL, 0);
  #endif
  #ifdef VESC_DEBUG_ELBOW
    Serial1.begin(115200, SERIAL_8N1, ELBOW_MOTOR_SERIAL, 0);
  #endif
#endif

  motorSetup();
  disableMotors();

  encoderSetup();

  delay(5000); // Let the motors calibrate themselves
  setMotorTorque({0,0});
  enableMotors();
  delay(5000); // Let the Motor controller initialize to 0


  controllerState = controllerState_e::HOMING;
  Serial.println("starting homing");
  homingRoutine();
  Serial.println("ending homing");
  controllerState = controllerState_e::IDLE;

  Serial.println("Setup complete");
}

void loop () { 
  unsigned long loop_start = micros();

  encoderPositions_t positions = getEncoderPositionsScaled();

  serial_sendEncoderPositions(positions);
  computer_commands_t commands = serial_getLatestComputerCommands();

  commands = safety_check(commands, positions);
  setMotorTorque(commands);

#ifdef VESC_DEBUG
  Serial.print("VESC: ");
  Serial.println(Serial1.readStringUntil('\n'));
#endif

  // use a smarter delay for loop aware control period timing
  uint32_t smart_delay = CONTROL_PERIOD_US - (micros() - loop_start);
  if (smart_delay < 0) smart_delay = 0;
  if (smart_delay > CONTROL_PERIOD_US) smart_delay = 0; // handle overflow since the previous line would never happen
  // Serial.print("smart_delay: ");
  // Serial.println(smart_delay);
  delayMicroseconds(smart_delay);
} 

