#include <stdint.h>
#pragma once

class Brushed
{
private:
    int FWDPin;
    int RVSPin;
    int rampTime;
    bool enPinMode;
    int enPin;
public:
    Brushed(int _FWDPin, int _RVSPin, int _enPin, bool _enPinMode);
    void rotate(int speed);
    void commandVelocity(int16_t velocity);
    void updateEncoderPos(int16_t _encoderPos);
    void teleopVelocity(int speed);
    int16_t encoderPos = 0;
};
