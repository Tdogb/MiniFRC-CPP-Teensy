#include <Arduino.h>
#include "Brushed.h"
#include <stdint.h>

Brushed::Brushed(int _FWDPin, int _RVSPin)
{
    FWDPin = _FWDPin;
    RVSPin = _RVSPin;
    pinMode(FWDPin, OUTPUT);
    pinMode(RVSPin, OUTPUT);
    analogWriteFrequency(FWDPin, 600);
    analogWriteFrequency(RVSPin, 600);
}

Brushed::~Brushed()
{
}

void Brushed::rotate(int speed) {
    // Serial.println("");
    // Serial.print("Rotate ");
    // Serial.println(FWDPin);
    // Serial.print(" ");
    // Serial.print(speed);
    // Serial.println("");
    if(speed > 128) {
        speed = 128;
    }
    if(speed < -128) {
        speed = -128;
    }
    if(speed > 0) {
        analogWrite(RVSPin, 0);
        //Serial.println(map(speed, 0, 100, 0, 256));
        analogWrite(FWDPin, speed*2);
    }
    else
    {
        analogWrite(FWDPin, 0);
        //Serial.println("reverse");
        analogWrite(RVSPin, abs(speed)*2);
    }
}

void Brushed::ramp() {
    
}