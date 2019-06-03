#include <Arduino.h>
#include "Brushed.h"
#include <stdint.h>

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
}

void Brushed::rotate(int speed) {
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

void Brushed::ramp() {
    
}