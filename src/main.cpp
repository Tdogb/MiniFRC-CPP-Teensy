#include "main.h"

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
  if(robotModeTimer.hasPassed(15000)) {
    
  }
  if(autoTimer.hasPassed(10)) {
    autoTimer.restart();
    drivetrain->followerUpdate();
    drivetrain->updateVelocityDrivetrain();
  }
}

//Drivetrain
void mainDrivetrainLoop() {
  drivetrain->encoderUpdate();
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
  switch (buttonID) {
  //ELEVATOR
  case LEVEL_DOWN:
    changeElevatorHeightState(false);
    break;
  case LEVEL_UP:
    changeElevatorHeightState(true);
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
  loopCount++;
}

void mainScheduledGameControllerLoop() {

}

void changeElevatorHeightState(bool increment) {
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
      if(increment){setpointElevatorRotation+=stepAmmount;} else {setpointElevatorRotation-=stepAmmount;}
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
}

void debugQueue() {
}

void debugLoop() {
}