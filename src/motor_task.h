#include <Arduino.h>
#include <AccelStepper.h>
#include "common_types.h"

extern commputer_commands_t serial_getLatestComputerCommands();

// Define some steppers and the pins the will use
AccelStepper linearStepper(AccelStepper::FULL2WIRE, 17, 2);
AccelStepper elbowStepper(AccelStepper::FULL2WIRE, 13, 12);

