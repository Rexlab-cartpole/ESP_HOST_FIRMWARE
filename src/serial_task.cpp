#include "serial_task.h"

/********** Private Vars *************/
commputer_commands_t computer_command;
const TickType_t xDelaySerial = 2 / portTICK_PERIOD_MS;

/********** Public Functions *************/
commputer_commands_t serial_getLatestComputerCommands(){
    return computer_command;
}

/********** Private Functions *************/
static void serial_sendEncoderPositions();
static void serial_readComputerCommands();



bool dbg = false;
void serial_task(void* pv){
    // while(1){
        serial_sendEncoderPositions();
        serial_readComputerCommands();

        // vTaskDelay(5);
        // vTaskDelay(xDelaySerial);
    // }
}


void serial_sendEncoderPositions(){
    encoderPositions_t position = encoder_getEncoderPositions(dbg);

    uint32_t start = micros();
    char buffer[100];
    uint8_t prescision = 3;
    
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

    dtostrf(computer_command.linearAccel, 5, 3, buffer);
    Serial.print(buffer);
    Serial.print(",");

    dtostrf(computer_command.elbowAccel, 5, 3, buffer);
    Serial.print(buffer);
    Serial.print(",");

    Serial.print(micros() - start);
    Serial.print(",");


    Serial.println();

}


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
    computer_command.linearAccel = atof(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    computer_command.elbowAccel = atof(strtokIndx);     // convert this part to a float

    // if (computer_command.linearAccel == 12.5 && computer_command.shoulderAccel == -13.5){
    //     dbg = true;
    // }
    // else{
    //     dbg = false;
    // }

}

void serial_readComputerCommands(){
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        newData = false;
    }
}

