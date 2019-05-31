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
#include <robot/util/Queue.h>
#include "robot/subsystems/Mechanisms.h"

#define robotSerial RobotSerial::Instance()
#define drivetrain Drivetrain::Instance()
#define mechanisms Mechanisms::Instance()

//2mm pitch. Every one rotation is 2mm up
int elevatorHeightsMM[] = {0,116,170};
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
void changeElevatorRotationState(bool increment);

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
  }
  if(radioSchedular.hasPassed(200)) {
    radioSchedular.restart();
    mainScheduledMechanismsLoop();
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
    else if(setpointElevatorHeight % 2 == 0) {
      height((setpointElevatorHeight-1)*CARGO_OFFSET);
    }
    else {
      height(setpointElevatorHeight*HATCH_OFFSET);
    }
    currentElevatorHeight = setpointElevatorHeight;
  }
  if(setpointElevatorRotation != currentElevatorRotation) {
    heading(setpointElevatorRotation);
    currentElevatorRotation = setpointElevatorRotation;
  }
}

//Mechanisms


// void updateMechanismStates(int buttonID) {
//   switch (buttonID)
//   {
//   case SQR_BUTTON:
//     mechanisms->setMechanismState(true,!mechanisms->previousBallState);
//     break;
  
//   default:
//     break;
//   }
// }

void mainMechanismsLoop() {
  
}

void mainScheduledMechanismsLoop() {
  //mechanisms->update();
  mechanisms->packet[ML-1] = 126;
  robotSerial->writeBT(mechanisms->packet);
}

//Game controller
ButtonPressCounter xButtonCounter;
ButtonPressCounter triButtonCounter;
ButtonPressCounter dpadLeftButtonCounter;
ButtonPressCounter dpadRightButtonCounter;

ButtonPressCounter sqrButtonCounter;
ButtonPressCounter oButtonCounter;
ButtonPressCounter intakeBall;
ButtonPressCounter outtakeBall;
ButtonPressCounter intakeHatch;
ButtonPressCounter outtakeHatch;

void buttonHandler(int buttonID) {
  Serial.println("");
  Serial.print("Button ID: ");
  Serial.print(buttonID);
  switch (buttonID) {
  //ELEVATOR
  case X_BUTTON:
    changeElevatorHeightState(false);
    break;
  case TRI_BUTTON:
    changeElevatorHeightState(true);
    break;
  case SQR_BUTTON:
    changeElevatorRotationState(true);
    break;
  case O_BUTTON:
    changeElevatorRotationState(false);
    break;
  //MECHANISMS
  case DPAD_LEFT:
    mechanisms->setMechanismState(true,!mechanisms->previousBallState);
    Serial.println("dpad up");
    break;
  case DPAD_RIGHT:
    mechanisms->setMechanismState(false,!mechanisms->previousHatchState);
    Serial.println("dpad down");
    break;
  case L2:
    mechanisms->setIntakeOuttake(true,true,false);
    break;
  case L1:
    mechanisms->setIntakeOuttake(true,false,true);
    break;
  case R2:
    mechanisms->setIntakeOuttake(false,true,false);
    break;
  case R1:
    mechanisms->setIntakeOuttake(false,false,true);
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
  //Mechanisms
  sqrButtonCounter.update(&buttonHandler, robotSerial->buttons[SQR_BUTTON], SQR_BUTTON);
  oButtonCounter.update(&buttonHandler, robotSerial->buttons[O_BUTTON], O_BUTTON);
  intakeBall.update(&buttonHandler, robotSerial->buttons[L2], L2);
  outtakeBall.update(&buttonHandler, robotSerial->buttons[L1], L1);
  intakeHatch.update(&buttonHandler, robotSerial->buttons[R2], R2);
  outtakeHatch.update(&buttonHandler, robotSerial->buttons[R1], R1);

  loopCount++;
}

void mainScheduledGameControllerLoop() {

}

void changeElevatorHeightState(bool increment) {
  // Serial.println("");
  // Serial.print("Setpoint: ");
  // Serial.print(setpointElevatorHeight);
  // Serial.print(" increment ");
  // Serial.print(increment);
  if(setpointElevatorHeight == 0) { if(increment){setpointElevatorHeight++;}  }
  else if(setpointElevatorHeight == CARGO_3) {  if(!increment){setpointElevatorHeight--;} }
  else {
      if(increment){setpointElevatorHeight++;} else {setpointElevatorHeight--;}
  }
}

void changeElevatorRotationState(bool increment) {
  //FOR TESTING
  //Serial.println("changeElevatorRotationState");
  if(setpointElevatorRotation == 0) { if(increment){setpointElevatorRotation+=10;}  }
  else {
      if(increment){setpointElevatorRotation+=10;} else {setpointElevatorRotation-=10;}
  }
}

void debugInit() {
  //Add elevator debug to queue
  //queue.addMeasuingValue(&debugVal);
  //queue.addToQueue(&debugElevator);
}

void debugQueue() {
  //queue.addToQueue(&debugElevator);
}

void debugLoop() {
  //queue.update();
}