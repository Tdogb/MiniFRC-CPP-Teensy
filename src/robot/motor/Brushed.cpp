#include <Arduino.h>
#include "Brushed.h"
#include <stdint.h>
#include <FastPID.h>

#define Kp 3
#define Ki 0.05
#define Kd 0
#define updateFreq 10000000

FastPID pid(Kp, Ki, Kd, updateFreq, 8, false);

Brushed::Brushed(int _FWDPin, int _RVSPin, int _enPin, bool _enPinMode)
{
    FWDPin = _FWDPin;
    RVSPin = _RVSPin;
    enPin = _enPin;
    enPinMode = _enPinMode;
    pinMode(FWDPin, OUTPUT);
    pinMode(RVSPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    if(enPinMode) {
        analogWriteFrequency(FWDPin, 600);
        analogWriteFrequency(RVSPin, 600);
    }
    else {
        analogWriteFrequency(FWDPin, 600);
        analogWriteFrequency(RVSPin, 600);
        digitalWrite(enPin, HIGH);
    }
    pid.setOutputRange(0, 128); //Making an assumption here when motors are reversed
}

void Brushed::rotate(int speed) {
    Serial.print(speed);
    Serial.print('\t');
    //Serial.print(" encoderPos: ");
    //Serial.print(encoderPos);
    // Serial.print(encoderPos);
    // Serial.print('\t');
    if(speed > 128) {
        speed = 128;
    }
    if(speed < -128) {
        speed = -128;
    }
    if(speed > 0) {
        if(enPinMode) {
            digitalWrite(RVSPin, LOW);
            digitalWrite(FWDPin, HIGH);
            analogWrite(enPin, speed*2);
        }
        else {
            analogWrite(RVSPin, 0);
            analogWrite(FWDPin, speed*2);
        }
    }
    else {
        if(enPinMode) {
            digitalWrite(FWDPin, LOW);
            digitalWrite(RVSPin, HIGH);
            analogWrite(enPin, abs(speed*2));
        }
        else {
            analogWrite(FWDPin, 0);
            analogWrite(RVSPin, abs(speed)*2);
        }
    }
}

void Brushed::commandVelocity(int16_t velocity) {
    int16_t absVelocity = abs(velocity);
    if(absVelocity > 0) {
        int16_t pidStep = pid.step(absVelocity, encoderPos);
        // Serial.print(" step: ");
        // Serial.print(pidStep);
        // Serial.print('\t');
        // Serial.print(" encoderPos: ");
        // Serial.print(encoderPos);
        // Serial.print(" ");
        rotate((velocity/absVelocity)*pidStep);
    }
    else {
        // Serial.print(0);
        // Serial.print('\t');
        rotate(0);
    }
}

void Brushed::updateEncoderPos(int16_t _encoderPos) {
    encoderPos = _encoderPos;
}


void Brushed::teleopVelocity(int _speed) {
    int speed = _speed;
    if(speed > 128) {
        speed = 128;
    }
    if(speed < -128) {
        speed = -128;
    }
    long mapped = map(speed,-128,128,-135,135);
    //Serial.println((int16_t)mapped);
    commandVelocity(mapped); //Have to find this
}
