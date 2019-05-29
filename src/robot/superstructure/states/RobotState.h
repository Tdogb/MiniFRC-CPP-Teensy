#ifndef ROBOT_H
#define ROBOT_H
#include "ElevatorState.h"
#include "MechanismState.h"
struct RobotState
{
    ElevatorState elevatorState;
    MechanismState ballIntakeState; 
    MechanismState hatchMechState;
};
#endif