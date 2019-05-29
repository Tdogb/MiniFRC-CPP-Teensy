// #include "BallIntakeSS.cpp"
// #include "HatchMechSS.cpp"
#include "MechanismSuperstructure.h"
class Supersturcture
{
private:
    BallIntakeSS ballIntakeState;
    HatchMechSS hatchMechState;
public:
    RobotState currentRobotState;
    RobotState desiredRobotState;
    Supersturcture(/* args */);
    void update(RobotState desiredState);
};

Supersturcture::Supersturcture(/* args */)
{
}

// void Supersturcture::update(RobotState desiredState) {
//     if(desiredState.ballIntakeState.theta != currentRobotState.ballIntakeState.theta) {
//         if(ballIntakeState.willCollide(desiredRobotState, desiredState.ballIntakeState)) {
//         }
//     }
// }
