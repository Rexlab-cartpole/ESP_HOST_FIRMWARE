#include <Arduino.h>
#include "motor_task.h"


void motor_task(void *pvParameters){
    linearStepper.setMaxSpeed(5000.0);
    elbowStepper.setMaxSpeed(5000.0);

    while(1){
        commputer_commands_t computerCommand = serial_getLatestComputerCommands();

        // if (computerCommand.linearAccel < 0){
        //     linearStepper.setSpeed(-4000);
        // }
        // else if(computerCommand.linearAccel > 0){
        //     linearStepper.setSpeed(4000);
        // }
        // else{
        //     linearStepper.setSpeed(0);
        // }

        // if (computerCommand.elbowAccel < 0){
        //     elbowStepper.setSpeed(-4000);
        // }
        // else if(computerCommand.elbowAccel > 0){
        //     elbowStepper.setSpeed(4000);
        // }
        // else{
        //     elbowStepper.setSpeed(0);
        // }

        // linearStepper.setAcceleration(computerCommand.linearAccel);
        // elbowStepper.setAcceleration(computerCommand.elbowAccel);


        if (computerCommand.linearAccel < 0){
            linearStepper.setAcceleration(-20000);
        }
        else if(computerCommand.linearAccel > 0){
            linearStepper.setAcceleration(20000);
        }
        else{
            // linearStepper.setAcceleration(0);
        }

        if (computerCommand.elbowAccel < 0){
            elbowStepper.setAcceleration(-20000);
        }
        else if(computerCommand.elbowAccel > 0){
            elbowStepper.setAcceleration(20000);
        }
        else{
            // elbowStepper.setAcceleration(0);
        }

        linearStepper.setSpeed(computerCommand.linearAccel);
        elbowStepper.setSpeed(computerCommand.elbowAccel);


        linearStepper.run();
        elbowStepper.run();

        // vTaskDelay(0);
    }
}

