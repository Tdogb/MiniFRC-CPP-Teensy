#include "Drivetrain.h"
#include <Arduino.h>
#include <stdint.h>
#include <cstdlib>
#include <FastPID.h>
#include <robot/sensor/RobotEncoder.h>

#define Kp 1
#define Ki 0.01
#define Kd 0.1
#define updateFreq 100

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
    if (std::abs(throttle) + std::abs(turn) > 10) {
        leftMotor->rotate(throttle+turn);
        rightMotor->rotate(throttle-turn);
    }
    else {
        leftMotor->rotate(0);
        rightMotor->rotate(0);
    }
}

void Drivetrain::debug() {
    updateEncoder();
    
}

void Drivetrain::debugScheduled() {
    pids();
    // Serial.println("");
    // Serial.print("Output: ");
    // Serial.print(readEncoder(true));
}

void Drivetrain::pids() {
    int16_t output = pidLeft.step(2000,(int16_t)(readEncoder(true)*100));
    Serial.println("");
    Serial.print("Left: ");
    Serial.print(readEncoder(true)*100);
    Serial.print(" Right: ");
    Serial.print(readEncoder(false)*100);
    //rightMotor->rotate(output);
}