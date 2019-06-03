#include "Elevator.h"
#include "robot/util/Queue.h"
#include "robot/defs.h"

Stepper rotationStepper(ROTATION_STEP_PIN, ROTATION_DIR_PIN); //step pin, dir pin
Stepper leadscrewStepper(LEADSCREW_STEP_PIN,LEADSCREW_DIR_PIN);
StepControl rotationController;
StepControl leadscrewController;
Queue heightQueue;
Queue rotationQueue;

bool leadscrewRunning = false;
bool rotationRunning = false;

int elevatorRotation = 0;
int elevatorRotationSetpoint = 0;
int elevatorHeight = 0;
int elevatorHeightSetpoint = 0;
int rd = 0; //rotation
int hts = 0; //height
bool numberSwitch = true;
int dist = 0;
int heightOffset = 0;
int previousHeading = 0;

void initElevator()
{
  rotationStepper.setAcceleration(900);
  rotationStepper.setMaxSpeed(2000);
  leadscrewStepper.setAcceleration(800);
  leadscrewStepper.setMaxSpeed(1000);
  // rotationStepper.setPosition(0);
  // leadscrewStepper.setPosition(0);
  rotationStepper.setTargetAbs(0);
  leadscrewStepper.setTargetAbs(0);
  heightQueue.addMeasuingValue(&leadscrewRunning);
  rotationQueue.addMeasuingValue(&rotationRunning);
}

void updateElevator() {
  leadscrewRunning = leadscrewController.isRunning();
  rotationRunning = rotationController.isRunning();
  heightQueue.update();
  rotationQueue.update();
  if(!rotationController.isOk() || !leadscrewController.isOk()) {
    Serial.println("TOO MANY FTM TIMERS USED");
  }
  if(!rotationController.isRunning() && (elevatorRotationSetpoint != rotationStepper.getPosition())) {
    rotationController.moveAsync(rotationStepper);
  }
  if(!leadscrewController.isRunning() && (elevatorHeightSetpoint != leadscrewStepper.getPosition())) {
    leadscrewController.moveAsync(leadscrewStepper);
  }
}

void height(int heightSetpoint) {
  heightQueue.addToQueue(&heightTask, heightSetpoint);
}

void heading(int headingSetpoint) {
  rotationQueue.addToQueue(&headingTask, headingSetpoint);
}

void headingTask(int headingSetpoint) {
  elevatorRotationSetpoint = headingSetpoint;
  if (headingSetpoint != previousHeading) {
    rotationStepper.setTargetAbs((int32_t)(elevatorRotationSetpoint)); //(double)(elevatorRotationSetpoint) * 2.6)
    if(-elevatorRotationSetpoint + heightOffset > 0) {
      leadscrewStepper.setTargetAbs((int32_t)(-elevatorRotationSetpoint + heightOffset));
    }
    previousHeading = headingSetpoint;
  }
}

void heightTask(int heightSetpoint) {
  elevatorHeightSetpoint = heightSetpoint;
  leadscrewStepper.setTargetAbs((int32_t)(elevatorHeightSetpoint));
  heightOffset = heightSetpoint;
}