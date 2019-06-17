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
Chrono autoTimer(Chrono::MILLIS);