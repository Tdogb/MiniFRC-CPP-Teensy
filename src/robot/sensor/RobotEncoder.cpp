#include <IntervalTimer.h>
#include <Encoder.h>
#include "RobotEncoder.h"
#include <robot/defs.h>
#include <avr/interrupt.h>
#include <avr/io.h>

//IntervalTimer encoderUpdate;
elapsedMicros time_left;
elapsedMicros time_right;
uint32_t time_elapsed_left = 0;
uint32_t time_elapsed_right = 0;
// volatile bool lastTick_left = 0;
// int lastRobotEncoderValue_left = 0;
// volatile bool lastTick_right = 0;
// int lastRobotEncoderValue_right = 0;
// const int cpr = 256;
volatile double encSpeed_right = 0;
volatile double encSpeed_left = 0;
// volatile double encRTime = 0;
// volatile double encLTime = 0;
// volatile uint32_t samples_right = 0;
// volatile uint32_t samples_left = 0;
// volatile uint32_t time_left_add = 0;
// volatile uint32_t time_right_add = 0;

void encoderInit()
{
    pinMode(L_ENC_A, INPUT);
    pinMode(L_ENC_B, INPUT);
    pinMode(R_ENC_A, INPUT);
    pinMode(R_ENC_B, INPUT);
    //encoderUpdate.begin(updateEncoder, 1);
    attachInterrupt(digitalPinToInterrupt(L_ENC_A), updateEncoderLeft, RISING);
    attachInterrupt(digitalPinToInterrupt(R_ENC_A), updateEncoderRight, RISING);
}

void updateEncoderLeft()
{
  // if(digitalReadFast(L_ENC_A) != lastTick_left) {
  //   lastTick_left = !lastTick_left;
  //   time_left_add += time_left;
  //   samples_left++;
  //   time_left = 0;
  // }
  // if(digitalReadFast(R_ENC_A) != lastTick_right) {
  //   lastTick_right = !lastTick_right;
  //   time_right_add += time_right;
  //   samples_right++;
  //   time_right = 0;
  // }
  time_elapsed_left = time_left;
  time_left = 0;
}

void updateEncoderRight() {
  time_elapsed_right = time_right;
  time_right = 0;
}

double readEncoder(bool leftSide) {
  //noInterrupts();
  //Serial.println(time_right_add/samples_right);
  //if(time_left_add != 0) {
    //encSpeed_left = (((2*PI)/(((double)(time_left_add/samples_left)/(double)100000)*cpr))/2); //((time_left/1000000)/cpr)/2; //rev/sec
    if(time_elapsed_left != 0) {
      encSpeed_left = (double)3900/(double)time_elapsed_left;
    }
    else {
      encSpeed_left = 0;
    }
    //time_elapsed_left = 0;
    // samples_left = 0;
    // time_left_add = 0; 
  // }
  //if(time_right_add != 0) {
    //encSpeed_right = (((2*PI)/(((double)(time_right_add/samples_right)/(double)100000)*cpr))/2); //((time_left/1000000)/cpr)/2; //rev/sec
    if(time_elapsed_right != 0) {
      encSpeed_right = (double)3900/(double)time_elapsed_right;
    }
    else {
      encSpeed_right = 0;
    }
    //time_elapsed_right = 0;
    // samples_right = 0;
    // time_right_add = 0;
  //}
  //interrupts();
  return (leftSide ? encSpeed_left : encSpeed_right);
    // Serial.println("");
    // Serial.print("Left side: ");
    // Serial.print(encSpeed_left);
    // Serial.print('\t');
    // Serial.print(" Right Side: ");
    // Serial.print(encSpeed_right);
    // return 0;
}