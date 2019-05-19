#include "RobotSerial.h"
#include <robot/defs.h>
#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

#define SERIALPORT Serial4

RobotSerial* RobotSerial::singleInstance = NULL;

RobotSerial* RobotSerial::Instance() {
    if(!singleInstance) {
        singleInstance = new RobotSerial();
    }
    return singleInstance;
}

RobotSerial::RobotSerial(/* args */)
{
    SERIALPORT.begin(38400);
}

void RobotSerial::update() {
    if (SERIALPORT.available() > 0) {
        SERIALPORT.readBytesUntil((char)255, packet, 6);
        lastControlPacket = millis();
        buttons[SQR_BUTTON] = bitRead(packet[4], 0); //Square button
        buttons[X_BUTTON] = bitRead(packet[4], 1); //X button
        buttons[O_BUTTON] = bitRead(packet[4], 2); // oButton
        buttons[TRI_BUTTON] = bitRead(packet[4], 3); //triangle button
        //Serial.println(buttons[X_BUTTON]);
        xLAxis = packet[0];
        yLAxis = packet[1];
        yRAxis = packet[2];
        xRAxis = packet[3];
        newData = true;
        // Serial.println("");
        // Serial.print("Throttle: ");
        // Serial.print(yLAxis);
        // Serial.print(" Turn: ");
        // Serial.print(xRAxis);
        // Serial.print(" buttonX: ");
        // Serial.print(xButton);
    }
}

double RobotSerial::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (double)(x - in_min) * (out_max - out_min) / (double)(in_max - in_min) + out_min;
}