#include "Follower.h"
#include <Chrono.h>
#include "robot/motor/Brushed.h"
#include "robot/defs.h"
#include <Arduino.h>

Brushed *motor;
std::vector<std::vector<double>> path;

Follower::Follower(std::vector<std::vector<double>> _path, size_t _pathSize, Brushed *_motor)
{
    motor = _motor;
    path = _path;
    pathSize = _pathSize;
    autoTimeStep = path[1][1] - path[0][1];
}

Follower::~Follower()
{
}

int16_t Follower::update() {
    // Serial.println("");
    // Serial.print("follower update: ");
    // Serial.print(time);
    if(time < pathSize) {
        int16_t commandedRPS = path[time][0] / WHEEL_DIA;
        // Serial.print(" CommandedRPS ");
        // Serial.print(commandedRPS);
        // Serial.print(" ");
        //motor->commandVelocity(commandedRPS);
        // Serial.print(" Commanded RPS: ");
        // Serial.print(commandedRPS);
        // Serial.print(" error: ");
        Serial.print(commandedRPS-motor->encoderPos);
        Serial.print(" ");
        previousSpeed = commandedRPS;
        time++;
    }
    else {
        //Serial.println("done");
        time = 0;
    }
    return previousSpeed;
}

