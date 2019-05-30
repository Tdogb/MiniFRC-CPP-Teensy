#include "RobotSerial.h"
#include <Arduino.h>
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
bool updateData = false;
int iterator = 0;
int8_t garbage = 0;
bool startFound = false;
void RobotSerial::update() {
    if(SERIALPORT.available() > 0) {
        if(!startFound) {
            int8_t in = SERIALPORT.read();
            Serial.println(in);
            if(in == 127) {
                startFound = true;
            }
        }
        else {
            if(iterator == 7) {
                garbage = SERIALPORT.read();
                updateData = true;
                iterator = 0;
                //Serial.println("");
            }
            else {
                packet[iterator] = SERIALPORT.read();
                //Serial.print(packet[iterator]);
                //Serial.print(" ");
                iterator++;
            }
        }
        if(updateData) {
            xLAxis = packet[0];
            yLAxis = packet[1];
            yRAxis = packet[2];
            xRAxis = packet[3];
            buttons[SQR_BUTTON] = bitRead(packet[4], 0); //Square button
            buttons[X_BUTTON] = bitRead(packet[4], 1); //X button
            buttons[O_BUTTON] = bitRead(packet[4], 2); // oButton
            buttons[TRI_BUTTON] = bitRead(packet[4], 3); //triangle button
            //Serial.println(buttons[X_BUTTON]);

            buttons[L1] = bitRead(packet[5], 0);
            buttons[L2] = bitRead(packet[5], 1);
            buttons[R1] = bitRead(packet[5], 2);
            buttons[R2] = bitRead(packet[5], 3);

            buttons[DPAD_UP] = bitRead(packet[6], 0);
            buttons[DPAD_RIGHT] = bitRead(packet[6], 1);
            buttons[DPAD_DOWN] = bitRead(packet[6], 2);
            buttons[DPAD_LEFT] = bitRead(packet[6], 3);

            newData = true;
            Serial.println("");
            Serial.print("Throttle: ");
            Serial.print(yLAxis);
            Serial.print(" Turn: ");
            Serial.print(xRAxis);
            updateData = false;
        }
    }

    // if (SERIALPORT.available() > 0) {
    //     SERIALPORT.readBytesUntil(255, packet, 7);
    //     //SERIALPORT.readBytes(packet, 8); // CHANGE THIS IF SIZE IS CHANGED, OR IF SIZEOF IS USED
    //     lastControlPacket = millis();
    //     xLAxis = packet[0];
    //     yLAxis = packet[1];
    //     yRAxis = packet[2];
    //     xRAxis = packet[3];
    //     buttons[SQR_BUTTON] = bitRead(packet[4], 0); //Square button
    //     buttons[X_BUTTON] = bitRead(packet[4], 1); //X button
    //     buttons[O_BUTTON] = bitRead(packet[4], 2); // oButton
    //     buttons[TRI_BUTTON] = bitRead(packet[4], 3); //triangle button
    //     //Serial.println(buttons[X_BUTTON]);

    //     buttons[L1] = bitRead(packet[5], 0);
    //     buttons[L2] = bitRead(packet[5], 1);
    //     buttons[R1] = bitRead(packet[5], 2);
    //     buttons[R2] = bitRead(packet[5], 3);

    //     buttons[DPAD_UP] = bitRead(packet[6], 0);
    //     buttons[DPAD_RIGHT] = bitRead(packet[6], 1);
    //     buttons[DPAD_DOWN] = bitRead(packet[6], 2);
    //     buttons[DPAD_LEFT] = bitRead(packet[6], 3);

    //     newData = true;
    //     Serial.println("");
    //     Serial.print("Throttle: ");
    //     Serial.print(yLAxis);
    //     Serial.print(" Turn: ");
    //     Serial.print(xRAxis);
    // }
}

double RobotSerial::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (double)(x - in_min) * (out_max - out_min) / (double)(in_max - in_min) + out_min;
}