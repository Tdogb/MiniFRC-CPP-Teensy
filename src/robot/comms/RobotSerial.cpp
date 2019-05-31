#include "RobotSerial.h"
#include <Arduino.h>
#include <robot/defs.h>
#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>
#include <robot/defs.h>
#include <robot/defs.h>

#define SERIALPORT Serial4

//uint8_t message[7] = {0,0,0,0,0,0,0xFE};
//ball_angl, hatch angl, all bits, End flag
int8_t message[ML] = {0,0,0,126};

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
    // while(!Serial) {}
    if(SERIALPORT.available() > 0) {
        if(startFound) {
            if(iterator == 7) {
                garbage = SERIALPORT.read();
                updateData = true;
                iterator = 0;
                //Serial.println("");
            }
            else {
                packet[iterator + 1] = SERIALPORT.read();
                //TEMP HOPEFULLY
                packet[0] = garbage;
                iterator++;
            }
            
        }
        else {
            int8_t in = SERIALPORT.read();
            //Serial.println(in);
            if(in == 127) {
                //Serial.println(in);
                startFound = true;
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

            buttons[DPAD_UP] = packet[6] == 1;
            buttons[DPAD_RIGHT] = packet[6] == 2;
            buttons[DPAD_DOWN] = packet[6] == 3;
            buttons[DPAD_LEFT] = packet[6] == 4;
            Serial.println("");
            Serial.print(packet[6]);

            //Serial.print(buttons[DPAD_UP]);
            // Serial.print(" ");
            // Serial.print(buttons[DPAD_DOWN]);

            newData = true;
            // Serial.println("");
            // Serial.print("Throttle: ");
            // Serial.print(yLAxis);
            // Serial.print(" Turn: ");
            // Serial.print(xRAxis);
            updateData = false;
        }
    }
}

double RobotSerial::mapDouble(double x, double in_min, double in_max, double out_min, double out_max) {
  return (double)(x - in_min) * (out_max - out_min) / (double)(in_max - in_min) + out_min;
}

void RobotSerial::writeBT(int8_t _message[]) {
    for(int i = 0; i < ML; i++) {
        message[i] = _message[i];
    }
    SERIALPORT.write((byte*)message, ML);
}