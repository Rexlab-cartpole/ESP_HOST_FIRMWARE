#include "encoder_task.h"

/********** Private Functions Forward Declaration *************/
void enableLinearEncoder();
void enableShoulderEncoder();
void enableElbowEncoder();
void processEncoderReadings();

/********** Private Vars *************/
encoderReadingsRaw_t encoderReadingsRaw;
encoderPositions_t encoderPositions;
AS5600 as5600_1;   //  use default Wire
AS5600 as5600_2;   //  use default Wire
AS5600 as5600_3;   //  use default Wire

const TickType_t xDelayEncoder = 5 / portTICK_PERIOD_MS;


/********** Public Functions *************/
void encoder_setup(){
    Wire.begin();

    pinMode(controlPin, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(controlPin3, OUTPUT);

    enableLinearEncoder();
    
    delay(100);
}

void encoder_task(void* pv){
    static int32_t _lastPosition;
    float temp;
    // while(1){
        enableLinearEncoder();
        delayMicroseconds(100);
        encoderReadingsRaw.linearRailPositionRaw = (lowPass2 * as5600_1.getCumulativePosition())
                                                   + (1.0f - lowPass2) * encoderReadingsRaw.linearRailPositionRaw;

        encoderReadingsRaw.linearRailVelocityRaw = (lowPass * as5600_1.getAngularSpeed(AS5600_MODE_RADIANS))
                                                   + (1.0f - lowPass) * encoderReadingsRaw.linearRailVelocityRaw;
        // delay(1);

        enableShoulderEncoder();
        delayMicroseconds(100);
        encoderReadingsRaw.shoulderPositionRaw = (lowPass2 * as5600_2.getCumulativePosition())
                                                 + (1.0f - lowPass2) * encoderReadingsRaw.shoulderPositionRaw;
        encoderReadingsRaw.shoulderVelocityRaw = (lowPass * as5600_2.getAngularSpeed(AS5600_MODE_RADIANS))
                                                 + (1.0f - lowPass) * encoderReadingsRaw.shoulderVelocityRaw;
        // delay(1);

        enableElbowEncoder();
        delayMicroseconds(100);
        encoderReadingsRaw.elbowPositionRaw = (lowPass2 * as5600_3.getCumulativePosition())
                                               + (1.0f - lowPass2) * encoderReadingsRaw.elbowPositionRaw;
        encoderReadingsRaw.elbowVelocityRaw = (lowPass * as5600_3.getAngularSpeed(AS5600_MODE_RADIANS))
                                              + (1.0f - lowPass) * encoderReadingsRaw.elbowVelocityRaw;


        processEncoderReadings();

    //     vTaskDelay(1);
    //     // vTaskDelay(xDelayEncoder);
    // }
}

encoderPositions_t encoder_getEncoderPositions(bool debug){
    return encoderPositions;
}


/********** Private Functions *************/

double calc_ang_velocity(float prev_val, float curr_val, float time_diff) {
    double delta_val = curr_val - prev_val;

    // Check for rollover case
    if (delta_val > M_PI) {
        delta_val -= 2 * M_PI;
    } else if (delta_val < -M_PI) {
        delta_val += 2 * M_PI;
    }

    double ang_velocity = delta_val / time_diff;

    return ang_velocity;
}

// this function prevents race conditions with partially updated encoder readings
void processEncoderReadings(){
    encoderPositions.elbowPosition = ((float) encoderReadingsRaw.elbowPositionRaw) * AS5600_RAW_TO_RADIANS;
    encoderPositions.shoulderPosition = ((float) encoderReadingsRaw.shoulderPositionRaw) * AS5600_RAW_TO_RADIANS;
    encoderPositions.linearRailPosition = ((float) encoderReadingsRaw.linearRailPositionRaw) * AS5600_RAW_TO_RADIANS;

    encoderPositions.elbowVelocity = encoderReadingsRaw.elbowVelocityRaw;
    encoderPositions.shoulderVelocity = encoderReadingsRaw.shoulderVelocityRaw;
    encoderPositions.linearRailVelocity = encoderReadingsRaw.linearRailVelocityRaw;
    
    // char buf[100];
    // sprintf(buf, "elbow: %f, shoulder: %f, linear: %f\n", encoderPositions.elbowPosition, encoderPositions.shoulderPosition, encoderPositions.linearRailPosition);
    // Serial.print(buf);
}

void enableShoulderEncoder(){
    digitalWrite(controlPin, HIGH); // active low
    digitalWrite(controlPin2, HIGH); // active low
    digitalWrite(controlPin3, HIGH); // active high // enabled here
}

void enableElbowEncoder(){
    digitalWrite(controlPin, LOW); // active low // enabled here
    digitalWrite(controlPin2, HIGH); // active low
    digitalWrite(controlPin3, LOW); // active high
}

void enableLinearEncoder(){
    digitalWrite(controlPin, HIGH); // active low
    digitalWrite(controlPin2, LOW); // active low // enabled here
    digitalWrite(controlPin3, LOW); // active high
}
