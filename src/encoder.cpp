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
    static encoderPositions_t lastPositions;
    static unsigned long lastTime = 0;

    encoderPositions_t positions;
    positions.elbowPosition = (elbowEncoder.getCount() * ELBOW_SCALE) + ELBOW_OFFSET;
    positions.linearRailPosition = (linearEncoder.getCount() * LINEAR_SCALE) + LINEAR_OFFSET;
    positions.shoulderPosition = (shoulderEncoder.getCount() * SHOULDER_SCALE) + SHOULDER_OFFSET;

    unsigned long elapsed_time = micros() - lastTime; 
    positions.elbowVelocity = 1000000 * (positions.elbowPosition - lastPositions.elbowPosition) / elapsed_time;
    positions.linearRailVelocity = 1000000 * (positions.linearRailPosition - lastPositions.linearRailPosition) / elapsed_time;
    positions.shoulderVelocity = 1000000 * (positions.shoulderPosition - lastPositions.shoulderPosition) / elapsed_time;
    
    // static variable memory saving
    lastPositions.elbowPosition = positions.elbowPosition;
    lastPositions.linearRailPosition = positions.linearRailPosition;
    lastPositions.shoulderPosition = positions.shoulderPosition; 
    lastTime = micros();

    return positions;
}


