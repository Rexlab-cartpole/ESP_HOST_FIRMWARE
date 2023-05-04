#include <Arduino.h>

struct encoderPositions_t{
    float linearRailPosition;
    float linearRailVelocity;
    float shoulderPosition;
    float shoulderVelocity;
    float elbowPosition;
    float elbowVelocity;
};

struct commputer_commands_t{
    float linearAccel;
    float elbowAccel;
};

struct encoderReadingsRaw_t{
    int32_t linearRailPositionRaw;
    int32_t shoulderPositionRaw;
    int32_t elbowPositionRaw;
    float linearRailVelocityRaw;
    float shoulderVelocityRaw;
    float elbowVelocityRaw;
};