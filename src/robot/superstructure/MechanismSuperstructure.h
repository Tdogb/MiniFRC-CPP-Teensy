#include "robot/superstructure/states/RobotState.h"

class MechanismSuperstructure 
{
private:
public:
    MechanismSuperstructure();
    virtual double timeToState(MechanismState state) { return 0; }
    virtual bool willCollide(RobotState robotState, MechanismState desiredState) { return true; }
    virtual RobotState willCollideElevatorResolution(ElevatorState elevatorState, MechanismState desiredState);
};

class HatchMechSS: public MechanismSuperstructure
{
private:
    /* data */
public:
    HatchMechSS(/* args */);
    bool willCollide(RobotState robotState, MechanismState desiredState) {
        return false;
    }
};

class BallIntakeSS: public MechanismSuperstructure
{
private:
    int drivebaseCollisionRange[2] = {45,135};
public:
    BallIntakeSS(/* args */);
    // BallIntakeSS::BallIntakeSS(/* args */) {
    // }

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

    // RobotState willCollideElevatorResolution(ElevatorState elevatorState, MechanismState desiredState) {

    // }
};