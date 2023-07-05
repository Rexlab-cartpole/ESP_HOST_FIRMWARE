#include <Arduino.h>
// #include "AccelStepper.h"
#include "FastAccelStepper.h"
#include "common_types.h"

extern commputer_commands_t serial_getLatestComputerCommands();

FastAccelStepperEngine engine = FastAccelStepperEngine();

FastAccelStepper *linearStepper = NULL;
FastAccelStepper *elbowStepper = NULL;

const int linearStepperPin1 = 13;
const int linearStepperPin2 = 12;

const int elbowStepperPin1 = 17;
const int elbowStepperPin2 = 2;

// Define some steppers and the pins the will use
// AccelStepper linearStepper(AccelStepper::FULL2WIRE, 17, 2);
// AccelStepper elbowStepper(AccelStepper::FULL2WIRE, 13, 12);