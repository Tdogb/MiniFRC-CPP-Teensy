#include "ElevatorState.cpp"
#include "MechanismState.cpp"

struct RobotState
{
    ElevatorState elevatorState;
    MechanismState ballIntakeState; 
    MechanismState hatchMechState;
};
