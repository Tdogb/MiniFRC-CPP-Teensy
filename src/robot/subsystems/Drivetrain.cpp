#include "Drivetrain.h"
#include <Arduino.h>
#include <stdint.h>

Brushed leftMotor(2,3);
Brushed rightMotor(4,7);

int8_t throttle = 0;
int8_t turn = 0;

void updateControls(int8_t throttleAxis, int8_t turningAxis) { //Replace with pointers and change serial to singleton
    //throttle = (int)(throttleAxis * (double)(100/128));
    //turn = (int)(turningAxis * (double)(100/258));
    throttle = throttleAxis;
    turn = turningAxis;
    //Serial.println(throttleAxis);
    //throttle = abs(throttle) > 25 ? throttle : 0;
    //turn = abs(turn) > 25 ? turn : 0;
    // Serial.println("");
    // Serial.print("newData ");
    // Serial.print(yRAxis);
    
}

void updateDrivetrain() {
    if (abs(throttle) + abs(turn) > 10) {
        leftMotor.rotate(throttle-turn);
        rightMotor.rotate(throttle+turn);
        // Serial.println("");
        // Serial.print("Left: ");
        // Serial.print((throttle-turn));
        // Serial.print(" Right: ");
        // Serial.print(throttle+turn);
    }
    else {
        leftMotor.rotate(0);
        rightMotor.rotate(0);
    }
}