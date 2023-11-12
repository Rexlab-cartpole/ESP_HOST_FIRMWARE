#include <ESP32Encoder.h> // https://github.com/madhephaestus/ESP32Encoder.git 
#include "common_types.h"


ESP32Encoder elbowEncoder;
ESP32Encoder linearEncoder;
ESP32Encoder shoulderEncoder;

void zeroLinearRailEncoder(){
  linearEncoder.setCount(0);
}

void encoderSetup(){
  elbowEncoder.attachFullQuad(EN_1_A, EN_1_B);
  linearEncoder.attachFullQuad(EN_2_A, EN_2_B);
  shoulderEncoder.attachFullQuad(EN_3_A, EN_3_B);

  elbowEncoder.setCount(0);
  linearEncoder.setCount(0);
  shoulderEncoder.setCount(0);
}

encoderPositions_t getEncoderPositionsScaled(){
    encoderPositions_t positions;

    positions.elbowPosition = (elbowEncoder.getCount() * ELBOW_SCALE) + ELBOW_OFFSET;
    positions.linearRailPosition = (linearEncoder.getCount() * LINEAR_SCALE) + LINEAR_OFFSET;
    positions.shoulderPosition = (shoulderEncoder.getCount() * SHOULDER_SCALE) + SHOULDER_OFFSET;

    // TODO: Update
    positions.elbowVelocity = 0.0f;
    positions.linearRailVelocity = 0.0f;
    positions.shoulderVelocity = 0.0f;

    return positions;
}


