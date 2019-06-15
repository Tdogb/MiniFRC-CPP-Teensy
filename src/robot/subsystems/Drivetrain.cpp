#include "Drivetrain.h"
#include <Arduino.h>
#include <stdint.h>
#include <cstdlib>
#include <FastPID.h>
#include <robot/sensor/RobotEncoder.h>

#define Kp 50
#define Ki 0
#define Kd 0
#define updateFreq 100000

FastPID pidLeft(Kp, Ki, Kd, updateFreq, 8, false);
FastPID pidRight(Kp, Ki, Kd, updateFreq, 8, false);

Drivetrain* Drivetrain::singleInstance = NULL;

Drivetrain* Drivetrain::Instance() {
    if(!singleInstance) {
        singleInstance = new Drivetrain();
    }
    return singleInstance;
}

Drivetrain::Drivetrain()
{
    encoderInit();
}

void Drivetrain::updateControls(int8_t throttleAxis, int8_t turningAxis) { //Replace with pointers and change serial to singleton
    throttle = throttleAxis;
    turn = turningAxis;
}

void Drivetrain::updateDrivetrain() {
    // if (std::abs(throttle) + std::abs(turn) > 10) {
    //     leftMotor->rotate(throttle+turn);
    //     rightMotor->rotate(throttle-turn);
    // }
    // else {
    //     leftMotor->rotate(0);
    //     rightMotor->rotate(0);
    // }
}

void Drivetrain::debug() {
    //updateEncoder();
}

void Drivetrain::debugScheduled() {
    // Serial.println("");
    // Serial.print("Output: ");
    // Serial.print(readEncoder(true));
}

void Drivetrain::pids() {
    int16_t encoderReadR = (int16_t)readEncoder(false);
    int16_t encoderReadL = (int16_t)readEncoder(true);
    int16_t outputL = pidLeft.step(75,encoderReadL);
    int16_t outputR = pidRight.step(75,encoderReadR);

    Serial.println("");
    Serial.print("Left: ");
    Serial.print(encoderReadL);
    Serial.print(" Right: ");
    Serial.print(encoderReadR);
    Serial.print(" PID output ");
    Serial.print(outputR);
    //rightMotor->rotate(outputL/2);
    //leftMotor->rotate(outputR/2); //at 75: 25khz oscil, 5000 encoder read.
    rightMotor->rotate(outputR);
}