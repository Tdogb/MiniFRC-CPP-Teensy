#include "MechanismSuperstructure.cpp"

class BallIntakeSS: public MechanismSuperstructure
{
private:
    int drivebaseCollisionRange[2] = {45,135};
public:
    BallIntakeSS(/* args */);
};

BallIntakeSS::BallIntakeSS(/* args */)
{
}

bool willCollide(RobotState robotState, MechanismState desiredState) {
    if(desiredState.theta < 0 && robotState.elevatorState.height < 10) {
        if(robotState.elevatorState.thetaRelDrivebase > 45 && robotState.elevatorState.thetaRelDrivebase < 135) {
            return true;
        }
    }
    else {
        return false;
    }
    return true;
}

RobotState willCollideElevatorResolution(ElevatorState elevatorState, MechanismState desiredState) {

}
