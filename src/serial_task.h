#include <Arduino.h>
#include "common_types.h"

extern encoderPositions_t encoder_getEncoderPositions(bool debug);

void serial_task(void *pvParameters);

commputer_commands_t serial_getLatestComputerCommands();

