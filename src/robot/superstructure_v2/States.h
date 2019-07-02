#ifndef __STATES_H__
#define __STATES_H__

struct ElevatorState
{
    int thetaRelDrivebase;
    int thetaRelWorld;
    int height;
};

struct MechanismState
{
    int theta;
    bool intakingGamepiece;
    bool hasGamepeice;
};

struct RobotState
{
    ElevatorState elevatorState;
    MechanismState ballIntakeState; 
    MechanismState hatchMechState;
} currentRobotState;

#endif // __STATES_H__