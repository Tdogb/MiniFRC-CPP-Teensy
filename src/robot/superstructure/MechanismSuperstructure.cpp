#include "states/RobotState.cpp"

class MechanismSuperstructure 
{
private:
public:
    MechanismSuperstructure();
    virtual double timeToState(MechanismState state) { return 0; }
    virtual bool willCollide(RobotState robotState, MechanismState desiredState) { return true; }
    virtual RobotState willCollideElevatorResolution(ElevatorState elevatorState, MechanismState desiredState);
};