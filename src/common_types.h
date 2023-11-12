#include <Arduino.h>

#define PI2 6.28318f

#define VESC_DEBUG 1 // Set to 1 to enable VESC debugging
#define VESC_DEBUG_LINEAR 1 // Set to 1 to enable VESC debugging for linear motor, 0 for elbow
// #define VESC_DEBUG_ELBOW 1

/*********** MOTOR DEFINITIONS *********/

// Pins
#define ENDSTOP 5
#define LINEAR_MOTOR_PWM 18
#define ELBOW_MOTOR_PWM 17
#define LINEAR_MOTOR_ENABLE 7
#define ELBOW_MOTOR_ENABLE 6
#ifdef VESC_DEBUG
    #define LINEAR_MOTOR_SERIAL 10
    #define ELBOW_MOTOR_SERIAL 11
#endif

// Constants
#define HOMING_SLOW_TORQUE 1.0f
#define HOMING_INITIAL_TORQUE 1.5f

#define MIN_TORQUE -10.0f
#define MAX_TORQUE 10.0f

#define MAX_ANGLE_SHOULDER 360 * 2
#define MIN_ANGLE_SHOULDER - 360 * 2

#define MAX_ANGLE_ELBOW 360 * 2
#define MIN_ANGLE_ELBOW - 360 * 2

#define MAX_LINEAR_POSITION 100 // CM
#define MIN_LINEAR_POSITION 0 // CM

/*********** CONTROLLER DEFINITIONS *********/

#define CONTROL_FREQ 100 // Hz
#define CONTROL_PERIOD_US 1000000/CONTROL_FREQ // us
#define CONTROL_TIMEOUT_MS 100 // ms

/*********** ENCODER DEFINITIONS *********/

// Elbow
#define EN_1_A 12 // CLK ENCODER 
#define EN_1_B 14 // DT ENCODER 

// Linear
#define EN_2_A 1 // CLK ENCODER 
#define EN_2_B 3 // DT ENCODER 

// Shoulder
#define EN_3_A 38 // CLK ENCODER 
#define EN_3_B 33 // DT ENCODER 

#define ELBOW_SCALE (1.0f/2400.0f) * PI2 // 2400 ticks per revolution
#define LINEAR_SCALE (1.0f/605) 
#define SHOULDER_SCALE (1.0f/2400.0f) * PI2 // 2400 ticks per revolution

#define ELBOW_OFFSET 1.5f * PI
#define LINEAR_OFFSET 0
#define SHOULDER_OFFSET 1.5f * PI

/*********** ENUMS & STRUCTS *********/

struct encoderPositions_t{
    float linearRailPosition; // in cm
    float linearRailVelocity; // in cm/s
    float shoulderPosition; // in degrees
    float shoulderVelocity; // in degrees/s
    float elbowPosition; // in degrees
    float elbowVelocity; // in degrees/s
};

struct computer_commands_t{
    float linearTorque;
    float elbowTorque;
};

struct encoderReadingsRaw_t{
    int32_t linearRailPositionRaw;
    int32_t shoulderPositionRaw;
    int32_t elbowPositionRaw;
    float linearRailVelocityRaw;
    float shoulderVelocityRaw;
    float elbowVelocityRaw;
};

enum controllerState_e{
    INIT,
    HOMING,
    IDLE,
    RUNNING,
    TIMEOUT,
    ERROR_HOMING_FAILED,
    ERROR_OVER_CURRENT,
    ERROR_PAST_ENDSTOP,
    ERROR_ENCODER_FAILURE,
};
