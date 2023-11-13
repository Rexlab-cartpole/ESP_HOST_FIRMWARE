#include <Arduino.h>
#include "common_types.h"

extern encoderPositions_t encoder_getEncoderPositions(bool debug);

computer_commands_t serial_getLatestComputerCommands();
static void serial_readComputerCommands();

/********** Private Vars *************/
static computer_commands_t computer_command;
const TickType_t xDelaySerial = 2 / portTICK_PERIOD_MS;
static int last_computer_command = 0;

/********** Public Functions *************/
computer_commands_t serial_getLatestComputerCommands(){
    serial_readComputerCommands();

    // timeout feature
    if (millis() - last_computer_command > CONTROL_TIMEOUT_MS){
        computer_command.linearTorque = 0;
        computer_command.elbowTorque = 0;
    }

    return computer_command;
}


void serial_sendEncoderPositions(encoderPositions_t position){
    char buffer[100];
    uint8_t prescision = 5;
    
    dtostrf(position.linearRailPosition, 5, prescision, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(position.linearRailVelocity, 5, prescision, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(position.shoulderPosition, 5, prescision, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(position.shoulderVelocity, 5, prescision, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(position.elbowPosition, 5, prescision, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(position.elbowVelocity, 5, prescision, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(computer_command.linearTorque, 5, 3, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(computer_command.elbowTorque, 5, 3, buffer);
    Serial.print(buffer);
    Serial.print(",");

    Serial.println();
}

/********** Private Functions *************/

// Serial parsing example from https://forum.arduino.cc/t/serial-input-basics-updated/382007/11
// ****** Serial parsing. Intentionally left minimally modified to preserve tracability to original source. ******

const byte numChars = 64;
static char receivedChars[numChars];
static char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
static boolean newData = false;

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
                Serial.flush();
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    computer_command.linearTorque = atof(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    computer_command.elbowTorque = atof(strtokIndx);     // convert this part to a float
}

void serial_readComputerCommands(){
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        newData = false;
        last_computer_command = millis();
    }
}
