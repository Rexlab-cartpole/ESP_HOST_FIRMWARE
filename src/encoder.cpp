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
  // elbowEncoder.attachHalfQuad(EN_1_A, EN_1_B);
  // linearEncoder.attachHalfQuad(EN_2_A, EN_2_B);
  // shoulderEncoder.attachHalfQuad(EN_3_A, EN_3_B);
  // elbowEncoder.attachSingleEdge(EN_1_A, EN_1_B);
  // linearEncoder.attachSingleEdge(EN_2_A, EN_2_B);
  // shoulderEncoder.attachSingleEdge(EN_3_A, EN_3_B);

  elbowEncoder.setCount(0);
  linearEncoder.setCount(0);
  shoulderEncoder.setCount(0);
}

encoderPositions_t getEncoderPositionsScaled(){
    static encoderPositions_t lastPositions;
    static unsigned long lastTime = 0;

    encoderPositions_t positions;
    positions.linearRailPosition = (linearEncoder.getCount() * LINEAR_SCALE) + LINEAR_OFFSET;
    positions.shoulderPosition = (shoulderEncoder.getCount() * SHOULDER_SCALE) + SHOULDER_OFFSET;
    positions.elbowPosition = (elbowEncoder.getCount() * ELBOW_SCALE) + ELBOW_OFFSET;

    unsigned long elapsed_time = micros() - lastTime;
    // doing this the long way for now to make sure it works + easier to debug
    // Shift velocities to the right (velocities[0] = newest, velocities[VEL_FILTER_BUF_SIZE-1] = oldest)
    for (int i=VEL_FILTER_BUF_SIZE-1; i > 0; i--) {
      positions.linearRailVelocities[i] = positions.linearRailVelocities[i-1];
      positions.shoulderVelocities[i] = positions.shoulderVelocities[i-1];
      positions.elbowVelocities[i] = positions.elbowVelocities[i-1];
    }
    positions.linearRailVelocities[0] = positions.linearRailPosition - lastPositions.linearRailPosition;
    positions.shoulderVelocities[0] = positions.shoulderPosition - lastPositions.shoulderPosition;
    positions.elbowVelocities[0] = positions.elbowPosition - lastPositions.elbowPosition;

    // Compute new velocity averages
    positions.linearRailVelocity = 0;
    positions.shoulderVelocity = 0;
    positions.elbowVelocity = 0;
    for (int i=0; i < VEL_FILTER_BUF_SIZE; i++) {
      positions.linearRailVelocity += positions.linearRailVelocities[i];
      positions.shoulderVelocity += positions.shoulderVelocities[i];
      positions.elbowVelocity += positions.elbowVelocities[i];
    }
    positions.linearRailVelocity /= (VEL_FILTER_BUF_SIZE);
    positions.shoulderVelocity /= (VEL_FILTER_BUF_SIZE);
    positions.elbowVelocity /= (VEL_FILTER_BUF_SIZE);

    // Scale averages based on elapsed time
    positions.linearRailVelocity = 1000000 * (positions.linearRailVelocity) / elapsed_time;
    positions.shoulderVelocity = 1000000 * (positions.shoulderVelocity) / elapsed_time;
    positions.elbowVelocity = 1000000 * (positions.elbowVelocity) / elapsed_time;
    
    // // Scale averages based on elapsed time
    // positions.linearRailVelocity = 1000000 * (positions.linearRailPosition - lastPositions.linearRailPosition) / elapsed_time;
    // positions.shoulderVelocity = 1000000 * (positions.shoulderPosition - lastPositions.shoulderPosition) / elapsed_time;
    // positions.elbowVelocity = 1000000 * (positions.elbowPosition - lastPositions.elbowPosition) / elapsed_time;
    
    // static variable memory saving
    lastPositions.linearRailPosition = positions.linearRailPosition;
    lastPositions.shoulderPosition = positions.shoulderPosition; 
    lastPositions.elbowPosition = positions.elbowPosition;
    lastTime = micros();

    // Serial.print("vels: "); Serial.print(positions.linearRailVelocity); Serial.print(", "); Serial.println(positions.shoulderVelocity);

    return positions;
}


