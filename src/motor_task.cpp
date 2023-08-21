#include <Arduino.h>
#include "motor_task.h"


void motor_task(void *pvParameters){
    engine.init();

    elbowStepper = engine.stepperConnectToPin(elbowStepperPin2);
    elbowStepper->setDirectionPin(elbowStepperPin1);
    linearStepper = engine.stepperConnectToPin(linearStepperPin2);
    linearStepper->setDirectionPin(linearStepperPin1);

    elbowStepper->setSpeedInHz(0);  // the parameter is us/step !!!
    elbowStepper->setAcceleration(100000);
    linearStepper->setSpeedInHz(0);  // the parameter is us/step !!!
    linearStepper->setAcceleration(100000);

    // // wiggle test
    // bool flag = true;
    // int count = 0;
    // const int thresh = 100;
    // while(1){
    //     if (flag){
    //         // linearStepper->move(1000);
    //         elbowStepper->move(1000);
    //         linearStepper->move(1000);
    //         flag = false;
    //         Serial.println("Moving forward");
    //         delay(2500);
    //     }
    //     else{
    //         // linearStepper->move(0);
    //         elbowStepper->move(-1000);
    //         linearStepper->move(-1000);
    //         flag = true;
    //         Serial.println("Moving backward");
    //         delay(2500);
    //     }
    // }

    float last_valid = 0.0f;
    
    while(1){
        commputer_commands_t computerCommand = serial_getLatestComputerCommands();
        if (computerCommand.linearAccel < 10.0f && computerCommand.linearAccel > -10.0f){
            computerCommand.linearAccel = last_valid;
        }
        else{
            last_valid = computerCommand.linearAccel; 
        }

        if (computerCommand.linearAccel < 0.0f){
            // linearStepper->stopMove();
            linearStepper->runBackward();
        }
        else if(computerCommand.linearAccel > 0.0f){
            // linearStepper->stopMove();
            linearStepper->runForward();
            // delay(1000);
        }
        else{
            // linearStepper->stopMove();
            // linearStepper->runForward();
        }

        if (computerCommand.elbowAccel < 0.0f){
            // elbowStepper->stopMove();
            elbowStepper->runBackward();
            // delay(1000);
        }
        else if(computerCommand.elbowAccel > 0.0f){
            // elbowStepper->stopMove();
            elbowStepper->runForward();
            // delay(1000);
        }
        else{
            // elbowStepper->stopMove();
            // linearStepper->runForward();
        }

        linearStepper->setSpeedInHz(abs(computerCommand.linearAccel));
        elbowStepper->setSpeedInHz(abs(computerCommand.elbowAccel));
        elbowStepper->setAcceleration(100000);
        linearStepper->setAcceleration(100000);
    }
}

