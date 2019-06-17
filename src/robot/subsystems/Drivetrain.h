#pragma once

#include <robot/motor/Brushed.h>
#include "robot/defs.h"
#include <stdint.h>
#include <Arduino.h>

class Drivetrain
{
private:
    Drivetrain();
    Brushed* leftMotor = new Brushed(L_MOTOR_FWD,L_MOTOR_RVS, L_MOTOR_EN, true);
    Brushed* rightMotor = new Brushed(R_MOTOR_FWD, R_MOTOR_RVS, R_MOTOR_EN, true);
    static Drivetrain* singleInstance;
    int leftMotorSpeed = 0;
    int rightMotorSpeed = 0;
public:
    static Drivetrain* Instance();
    void pids();
    void updateControls(int8_t throttleAxis, int8_t turningAxis);
    void updateDrivetrain();
    void updateVelocityDrivetrain();
    void encoderUpdate();
    void debug();
    void debugScheduled();
    void followerUpdate();

    int8_t throttle = 0;
    int8_t turn = 0;
    bool inAuto = false;
};