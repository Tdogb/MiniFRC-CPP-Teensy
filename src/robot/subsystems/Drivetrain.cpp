#include "Drivetrain.h"
#include <Arduino.h>
#include <stdint.h>
#include <cstdlib>
#include <FastPID.h>
#include <robot/sensor/RobotEncoder.h>
#include "robot/autonomous/Follower.h"
#include "robot/autonomous/Path.h"
#include "robot/util/RobotMode.h"

Follower* leftFollower;
Follower* rightFollower;

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
    leftFollower = new Follower(pathLeft, pathLeft.size(), leftMotor);
    rightFollower = new Follower(pathRight, pathRight.size(), rightMotor);
}

void Drivetrain::updateControls(int8_t throttleAxis, int8_t turningAxis) { //Replace with pointers and change serial to singleton
    throttle = throttleAxis;
    turn = turningAxis;
}

void Drivetrain::updateDrivetrain() {
    if(!inAuto) {
        if (std::abs(throttle) + std::abs(turn) > 10) {
            leftMotorSpeed = throttle+turn;
            rightMotorSpeed = throttle-turn;
        }
        else {
            leftMotorSpeed = 0;
            rightMotorSpeed = 0;
        }
    }
}

void Drivetrain::updateVelocityDrivetrain() {
    Serial.println("");
    if(inAuto) {
        leftMotor->commandVelocity(leftMotorSpeed);
        rightMotor->commandVelocity(rightMotorSpeed);
    }
    else {
        leftMotor->teleopVelocity(leftMotorSpeed);
        rightMotor->teleopVelocity(rightMotorSpeed);
    }
}

void Drivetrain::encoderUpdate() {
    leftMotor->updateEncoderPos((int16_t)readEncoder(true));
    rightMotor->updateEncoderPos((int16_t)readEncoder(false));
}

void Drivetrain::followerUpdate() {
    if(inAuto) {
        leftMotorSpeed = leftFollower->update();
        rightMotorSpeed = rightFollower->update();
    }
}

void Drivetrain::debug() {
}

void Drivetrain::debugScheduled() {

}

void Drivetrain::pids() {

}