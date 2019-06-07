#include <IntervalTimer.h>
#include <Encoder.h>
#include "RobotEncoder.h"
#include <robot/defs.h>

IntervalTimer encoderUpdate;
elapsedMicros time_left;
elapsedMicros time_right;
volatile bool lastTick_left = 0;
int lastRobotEncoderValue_left = 0;
volatile bool lastTick_right = 0;
int lastRobotEncoderValue_right = 0;
const int cpr = 256;
volatile double encSpeed_right = 0;
volatile double encSpeed_left = 0;

void encoderInit()
{
    pinMode(L_ENC_A, INPUT);
    pinMode(R_ENC_A, INPUT);
    encoderUpdate.begin(updateEncoder, 10);
}

void updateEncoder()
{
  //encSpeed_left = digitalReadFast(L_ENC_A);
  if(digitalReadFast(L_ENC_A) != lastTick_left) {
    encSpeed_left = (((2*PI)/(((double)(time_left)/(double)100000)*cpr))/2); //((time_left/1000000)/cpr)/2; //rev/sec
    lastTick_left = !lastTick_left;
    time_left = 0;
  }
  if(digitalReadFast(R_ENC_A) != lastTick_right) {
    encSpeed_right = (((2*PI)/(((double)(time_right)/(double)100000)*cpr))/2); //((time_left/1000000)/cpr)/2; //rev/sec
    lastTick_right = !lastTick_right;
    time_right = 0;
  }
}

double readEncoder(bool leftSide) {
  return (leftSide ? encSpeed_left : encSpeed_right);
    // Serial.println("");
    // Serial.print("Left side: ");
    // Serial.print(encSpeed_left);
    // Serial.print('\t');
    // Serial.print(" Right Side: ");
    // Serial.print(encSpeed_right);
    // return 0;
}