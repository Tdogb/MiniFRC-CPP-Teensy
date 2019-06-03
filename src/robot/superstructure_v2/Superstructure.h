#include "States.h"

class Superstructure
{
private:
    MechanismState currentBallIntakeState;
    MechanismState currentHatchState;
    ElevatorState currentElevatorState;

    MechanismState desiredBallIntakeState;
    MechanismState desiredHatchState;
    ElevatorState desiredElevatorState;
    void elevatorHeightBallIntakeCoordination();
    void elevatorThetaBallIntakeCoordination();

public:
    Superstructure(/* args */);
    void update(MechanismState ballIntakeState, ElevatorState elevatorState);

};
