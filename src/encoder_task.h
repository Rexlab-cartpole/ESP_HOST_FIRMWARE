#include <Arduino.h>
#include "common_types.h"
#include "AS5600.h"
#include "Wire.h"


const int controlPin = 25; // a active low
const int controlPin2 = 26; // b active low
const int controlPin3 = 27; // c active high
const float PI2 = 2.0f * PI;
const float REV_THRES_UP = 0.9f * PI2;
const float REV_THRES_DOWN = 0.1f * PI2;

const float ENCODER_LINEAR_ANGULAR_RATIO = 0.00602f;
const float ENCODER_ELBOW_ANGULAR_RATIO= 0.2f;
const float ENCODER_SHOULDER_ANGULAR_RATIO = 1.0f;

const float lowPass3 = 0.05f;
const float lowPass = 0.4f;
const float lowPass2 = 0.3f;

void encoder_setup();
void encoder_task(void *pvParameters);
encoderPositions_t encoder_getEncoderPositions(bool debug);