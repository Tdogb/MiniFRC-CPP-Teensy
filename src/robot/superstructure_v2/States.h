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

//MechanismState ballIntakeScoring;

// void getStartingState() {
//     RobotState startingState;
//     startingState.ballIntakeState.theta = 0;
// }

// void l1() {

// }

// void l2() {

// }

// void l3() {

// }