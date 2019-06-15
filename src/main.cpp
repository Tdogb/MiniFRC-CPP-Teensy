#include <Arduino.h>
#include <vector>
#include <Chrono.h>
#include <IntervalTimer.h>
#include <robot/defs.h>
#include <robot/comms/RobotSerial.h>
#include <robot/sensor/RobotEncoder.h>
#include <robot/subsystems/Elevator.h>
#include <TeensyStep.h>
#include <robot/subsystems/Drivetrain.h>
#include <robot/util/ButtonPressCounter.h>
#include "robot/util/ButtonHoldCounter.h"
#include <robot/util/Queue.h>
#include "robot/subsystems/Mechanisms.h"
//#include "robot/superstructure_v2/Superstructure.h"

#define robotSerial RobotSerial::Instance()
#define drivetrain Drivetrain::Instance()
#define mechanisms Mechanisms::Instance()

//2mm pitch. Every one rotation is 2mm up
int elevatorHeightsMM[] = {0,232,1000,4000}; //11600, 17000
//0, hatch cargo
#define HATCH_OFFSET 232
#define CARGO_OFFSET 340

#define GROUND 0
#define HATCH_1 1
#define CARGO_1 2
#define HATCH_2 3
#define CARGO_2 4
#define HATCH_3 5
#define CARGO_3 6
//int elevatorHeightsConverted[] = {0,46400,68000}; //steps

int currentElevatorHeight = 0;
int setpointElevatorHeight = 0;
int currentElevatorRotation = 0;
int setpointElevatorRotation = 0;

bool debugVal = false;

void mainElevatorInit();

void mainDrivetrainLoop();
void mainElevatorLoop();
void mainMechanismsLoop();
void mainGameControllerLoop();

void mainScheduledDrivetrainLoop();
void mainScheduledElevatorLoop();
void mainScheduledMechanismsLoop();
void mainScheduledGameControllerLoop();

void buttonHandler();
void dpadHandler();
void buttonSetup();
void updateMechanismStates(int buttonID);

void changeElevatorHeightState(bool increment);
void changeElevatorRotationState(bool increment, int stepAmmount);
void avoidCollisionRotation();

void debugInit();
void debugLoop();
void debugQueue();

long averageThrottle = 0;
long averageTurn = 0;
long triangleButtonPresses = 0;
long squareButtonPresses = 0;
long oButtonPresses = 0;
long xButtonPresses = 0;
long loopCount = 0;

#define updateClockRate 100

Chrono schedular(Chrono::MILLIS);
Chrono radioSchedular(Chrono::MILLIS);
Chrono pidLoop(Chrono::MICROS);

void setup() {
  Serial.begin(9600);
  initElevator();
  debugInit();
}

void loop() {
  mainGameControllerLoop();
  mainDrivetrainLoop();
  mainElevatorLoop();
  mainMechanismsLoop();
  debugLoop();
  if(schedular.hasPassed(updateClockRate)) {
    schedular.restart();
    mainScheduledGameControllerLoop();
    mainScheduledElevatorLoop();
    mainScheduledDrivetrainLoop();
    debugQueue();
  }
  if(radioSchedular.hasPassed(200)) {
    radioSchedular.restart();
    mainScheduledMechanismsLoop();
  }
  if(pidLoop.hasPassed(10)) {
    drivetrain->pids();
  }
}

//Drivetrain
void mainDrivetrainLoop() {
  drivetrain->debug();
}
void mainScheduledDrivetrainLoop() {
  drivetrain->debugScheduled();
  drivetrain->updateControls(averageThrottle/loopCount, averageTurn/loopCount);
  drivetrain->updateDrivetrain();
  loopCount = 0;
  averageThrottle = 0;
  averageTurn = 0;
}

//Elevator

void mainElevatorLoop() {
  updateElevator();
}
void mainScheduledElevatorLoop() {
  if(setpointElevatorHeight != currentElevatorHeight) {
    if(setpointElevatorHeight == 0) {
      height(0);
    }
    // else if(setpointElevatorHeight % 2 == 0) {
    //   //height((setpointElevatorHeight-1)*CARGO_OFFSET);
    //   height(elevatorHeightsMM[setpointElevatorHeight-1]);
    // }
    else {
      //height(setpointElevatorHeight*HATCH_OFFSET);
      height(elevatorHeightsMM[setpointElevatorHeight]);

    }
    currentElevatorHeight = setpointElevatorHeight;
  }
  if(setpointElevatorRotation != currentElevatorRotation) {
    heading(setpointElevatorRotation);
    currentElevatorRotation = setpointElevatorRotation;
  }
}

//Mechanisms

void mainMechanismsLoop() {
  
}

void mainScheduledMechanismsLoop() {
  //mechanisms->update();
  mechanisms->packet[ML-1] = 126;
  //Serial.println("");
  for(int i = 0; i < ML; i++) {
    //Serial.print(mechanisms->packet[i]);
    //Serial.print(" ");
  }
  robotSerial->writeBT(mechanisms->packet);
}
//Game controller
ButtonPressCounter xButtonCounter;
ButtonPressCounter triButtonCounter;
ButtonPressCounter dpadLeftButtonCounter;
ButtonPressCounter dpadRightButtonCounter;
ButtonPressCounter elevatorLevelDown;
ButtonPressCounter elevatorLevelUp;

ButtonPressCounter sqrButtonCounter;
ButtonPressCounter oButtonCounter;
ButtonHoldCounter intakeBall;
ButtonPressCounter outtakeBall;
ButtonPressCounter intakeHatch;
ButtonPressCounter outtakeHatch;

void buttonHandler(int buttonID) {
  //Serial.println("");
  //Serial.print("Button ID: ");
  //Serial.print(buttonID);
  switch (buttonID) {
  //ELEVATOR
  case LEVEL_DOWN:
    changeElevatorHeightState(false);
    //debug temp
    //mechanisms->setMechanismState(false,!mechanisms->previousHatchState);
    break;
  case LEVEL_UP:
    changeElevatorHeightState(true);
    //debug temp
    //mechanisms->setMechanismState(true,!mechanisms->previousBallState);
    break;
  case ROTATION_LEFT_FINE:
    changeElevatorRotationState(true, 10);
    break;
  case ROTATION_RIGHT_FINE:
    changeElevatorRotationState(false, 10);
    break;
  case ROTATION_LEFT_COURSE:
    changeElevatorRotationState(true, 130);
    break;
  case ROTATION_RIGHT_COURSE:
    changeElevatorRotationState(false, 130);
    break;
  //MECHANISMS
  case TRI_BUTTON:
    //mechanisms->setMechanismState(false,false, -1);
    zero();
    break;
  default:
    break;
  }
}

void mainGameControllerLoop() {
  robotSerial->update();
  averageThrottle += robotSerial->yLAxis;
  averageTurn += robotSerial->xRAxis;
  //Elevator
  xButtonCounter.update(&buttonHandler, robotSerial->buttons[X_BUTTON], X_BUTTON);
  triButtonCounter.update(&buttonHandler, robotSerial->buttons[TRI_BUTTON], TRI_BUTTON);
  dpadLeftButtonCounter.update(&buttonHandler, robotSerial->buttons[DPAD_LEFT], DPAD_LEFT);
  dpadRightButtonCounter.update(&buttonHandler, robotSerial->buttons[DPAD_RIGHT], DPAD_RIGHT);
  elevatorLevelUp.update(&buttonHandler, robotSerial->buttons[LEVEL_UP], LEVEL_UP);
  elevatorLevelDown.update(&buttonHandler, robotSerial->buttons[LEVEL_DOWN], LEVEL_DOWN);
  //Mechanisms
  sqrButtonCounter.update(&buttonHandler, robotSerial->buttons[SQR_BUTTON], SQR_BUTTON);
  oButtonCounter.update(&buttonHandler, robotSerial->buttons[O_BUTTON], O_BUTTON);

  mechanisms->setIntakeOuttake(true,robotSerial->buttons[BALL_INTAKE], robotSerial->buttons[BALL_OUTTAKE]);
  mechanisms->setIntakeOuttake(false, robotSerial->buttons[HATCH_INTAKE], robotSerial->buttons[HATCH_OUTTAKE]);
  //intakeBall.update(&buttonHandler, robotSerial->buttons[BALL_INTAKE], BALL_INTAKE);
  //outtakeBall.update(&buttonHandler, robotSerial->buttons[BALL_OUTTAKE], BALL_OUTTAKE);
  //intakeHatch.update(&buttonHandler, robotSerial->buttons[HATCH_INTAKE], HATCH_INTAKE);
  //outtakeHatch.update(&buttonHandler, robotSerial->buttons[HATCH_OUTTAKE], HATCH_OUTTAKE);

  loopCount++;
}

void mainScheduledGameControllerLoop() {

}

void changeElevatorHeightState(bool increment) {
  //mechanisms->setMechanismState(false,false, -1);
  if(setpointElevatorHeight == 0) {
     if(increment){
       setpointElevatorHeight++;
     }  
  }
  else if(setpointElevatorHeight == sizeof(elevatorHeightsMM)) {  if(!increment){setpointElevatorHeight--;} }
  else {
      if(increment){setpointElevatorHeight++;} else {setpointElevatorHeight--;}
  }
  if(setpointElevatorHeight == 0) {
    mechanisms->setMechanismState(true,true, -1);
    avoidCollisionRotation();
  }
  else if(setpointElevatorHeight == 1) {
    mechanisms->setMechanismState(true,false, 50);
  }
  else if(setpointElevatorHeight == 2) {
    mechanisms->setMechanismState(true,false, 80);
  }
  else if(setpointElevatorHeight == 3) {

  }
  avoidCollisionRotation();
}

void changeElevatorRotationState(bool increment, int stepAmmount) { //130 FOR COURSE
  // if(setpointElevatorRotation == 0) {
  //    if(increment){
  //      setpointElevatorRotation+=stepAmmount;
  //    }  
  // }
  // else {
      if(increment){setpointElevatorRotation+=stepAmmount;} else {setpointElevatorRotation-=stepAmmount;}
  // }
}
/*
1135
95
*/
void avoidCollisionRotation() {
  int relSteps = setpointElevatorRotation % 520;
  if(setpointElevatorHeight == GROUND) {
     if(relSteps < 130 && relSteps > 90) {
       setpointElevatorRotation = setpointElevatorRotation - (relSteps - 110);
     } 
     else if(relSteps > -130 && relSteps < -90) {
       setpointElevatorRotation = setpointElevatorRotation - (relSteps - 110);
     }
  }
}

void debugInit() {
  //Add elevator debug to queue
  //queue.addMeasuingValue(&debugVal);
  //queue.addToQueue(&debugElevator);
}

void debugQueue() {
}

void debugLoop() {
  //queue.update();
}