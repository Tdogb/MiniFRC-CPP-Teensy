#include <IntervalTimer.h>
#include <Encoder.h>
#include "RobotEncoder.h"

#define encPinALeft 28
#define encPinARight 27

IntervalTimer encoderUpdate;
elapsedMicros time_left;
elapsedMicros time_right;
volatile bool lastTick_left = 0;
int lastRobotEncoderValue_left = 0;
volatile bool lastTick_right = 0;
int lastRobotEncoderValue_right = 0;
const int cpr = 256;
volatile int encSpeed_right = 0;
volatile int encSpeed_left = 0;

void init()
{
    pinMode(encPinALeft, INPUT);
    pinMode(encPinARight, INPUT);
    encoderUpdate.begin(updateEncoder, 10);
}

void updateEncoder()
{
  //encSpeed_left = digitalReadFast(encPinALeft);
  if(digitalReadFast(encPinALeft) != lastTick_left) {
    encSpeed_left = (int)(((2*PI)/(((double)(time_left)/(double)100000)*cpr))/2); //((time_left/1000000)/cpr)/2; //rev/sec
    lastTick_left = !lastTick_left;
    time_left = 0;
  }
  if(digitalReadFast(encPinARight) != lastTick_right) {
    encSpeed_right = (int)(((2*PI)/(((double)(time_right)/(double)100000)*cpr))/2); //((time_left/1000000)/cpr)/2; //rev/sec
    lastTick_right = !lastTick_right;
    time_right = 0;
  }
}