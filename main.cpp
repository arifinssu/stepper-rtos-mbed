#include "mbed.h"
#include "mbedSerial.h"
#include "FlexyStepper.h"
#include <chrono>
#include <cstdint>

#define CONVEYOR_PWM_PIN        PB_0
#define CONVEYOR_FWD_PIN        PC_4
#define CONVEYOR_REV_PIN        PA_6

#define ROTATOR_PWM_PIN         PB_1
#define ROTATOR_FWD_PIN         PC_5
#define ROTATOR_REV_PIN         PA_7

#define UPRTR_SERVO_PIN         PC_9

#define SERIAL_TX_PIN           PB_6
#define SERIAL_RX_PIN           PB_7

FlexyStepper stepper(ROTATOR_PWM_PIN, ROTATOR_FWD_PIN);
FlexyStepper stepper2(CONVEYOR_PWM_PIN, CONVEYOR_FWD_PIN);
FlexyStepper stepper3(PB_4, PB_5);
FlexyStepper stepper4(PB_9, PB_10);
FlexyStepper stepper5(PB_8, PB_12);
FlexyStepper stepper6(PB_13, PB_14);

MbedSerial Serial(USBTX, USBRX);
String dataIn;

// Thread task1(osPriorityNormal, 1024);
// Thread task2(osPriorityNormal, 1024);
Thread task1;
Thread task2;
// bool executed = 1;

bool checkAllStepper()
{
    int res = 0;

    res += !stepper.processMovement();
    res += !stepper2.processMovement();
    res += !stepper3.processMovement();
    res += !stepper4.processMovement();
    res += !stepper5.processMovement();
    res += !stepper6.processMovement();

    if (res > 0)
    {
        return 0;
    }
    return 1;
}

void execute_stepper()
{
    while(!checkAllStepper());

    Serial.println("done");
}

void t2()
{
    while(1)
    {
        // if (executed == 0)
        // {
        if (!checkAllStepper())
        {
            Serial.println("stepper start");
            execute_stepper();
            // osThreadSetPriority(task2.get_id(), osPriorityIdle);
        // }
        }
        osThreadYield();
    }
}

void t1()
{
    while(1)
    {
        if (Serial.available() > 0) 
        {
            char inChar = (char)Serial.read();
            if (inChar != '\n') dataIn += inChar;
            else 
            {
                String hasil = dataIn;
                dataIn = "";
                Serial.println(hasil);
                if (hasil == "r")
                {
                    HAL_NVIC_SystemReset();
                }
                else if (hasil == "p")
                {
                    Serial.println(String(task1.get_priority()) + " " + String(task1.get_state()));
                    Serial.println(String(task2.get_priority()) + " " + String(task2.get_state()));
                }
                else if (hasil == "1")
                {
                    Serial.println("execute stepper");
                    stepper.setTargetPositionInSteps(1600);
                    stepper2.setTargetPositionInSteps(1600);
                    stepper3.setTargetPositionInSteps(1600);
                    stepper4.setTargetPositionInSteps(1600);
                    stepper5.setTargetPositionInSteps(1600);
                    stepper6.setTargetPositionInSteps(1600);
                    // osThreadSetPriority(task2.get_id(), osPriorityAboveNormal1);
                    // executed = 0;
                }
                else if (hasil == "2")
                {
                    Serial.println("back stepper");
                    stepper.setTargetPositionInSteps(0);
                    stepper2.setTargetPositionInSteps(0);
                    stepper3.setTargetPositionInSteps(0);
                    stepper4.setTargetPositionInSteps(0);
                    stepper5.setTargetPositionInSteps(0);
                    stepper6.setTargetPositionInSteps(0);
                    // osThreadSetPriority(task2.get_id(), osPriorityAboveNormal1);
                    // executed = 0;
                }
            }
        }
        osThreadYield();
    }
}

int main()
{
    Serial.begin(9600);
    Serial.println("debug");

    stepper.setStepsPerRevolution(800);
    stepper.setAccelerationInStepsPerSecondPerSecond(800);
    stepper.setSpeedInStepsPerSecond(800);

    stepper2.setStepsPerRevolution(800);
    stepper2.setAccelerationInStepsPerSecondPerSecond(800);
    stepper2.setSpeedInStepsPerSecond(800);

    stepper3.setStepsPerRevolution(800);
    stepper3.setAccelerationInStepsPerSecondPerSecond(800);
    stepper3.setSpeedInStepsPerSecond(800);

    stepper4.setStepsPerRevolution(800);
    stepper4.setAccelerationInStepsPerSecondPerSecond(800);
    stepper4.setSpeedInStepsPerSecond(800);

    stepper5.setStepsPerRevolution(800);
    stepper5.setAccelerationInStepsPerSecondPerSecond(800);
    stepper5.setSpeedInStepsPerSecond(800);

    stepper6.setStepsPerRevolution(800);
    stepper6.setAccelerationInStepsPerSecondPerSecond(800);
    stepper6.setSpeedInStepsPerSecond(800);

    // task1.start(&t1);
    task2.start(&t2);

    // osThreadSetPriority(task1.get_id(), osPriorityNormal1);
    // osThreadSetPriority(task2.get_id(), osPriorityIdle);
    // task1.join();
    // task2.join();

    t1();

    HAL_Delay(osWaitForever);
}