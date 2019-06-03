#include "Superstructure.h"

Superstructure::Superstructure(/* args */)
{

}

void Superstructure::update(MechanismState ballIntakeState, ElevatorState elevatorState) {
    desiredBallIntakeState = ballIntakeState;
    desiredElevatorState = elevatorState;
    if(ballIntakeState.theta != currentBallIntakeState.theta || elevatorState.height != currentElevatorState.height) {
        elevatorHeightBallIntakeCoordination();
    }
    if(elevatorState.thetaRelDrivebase != currentElevatorState.thetaRelDrivebase) {
        elevatorThetaBallIntakeCoordination();
    }
    currentBallIntakeState = desiredBallIntakeState;
    currentElevatorState = desiredElevatorState;
}

/*
Elevator ball intake height coordination
if elevator above (threshold) move ball intake to negative theta, move elevator down to set height
else if elevator is below threshold, move ball intake to correct theta if nessesary

Elevator ball intake rotation coordination
if elevator below threshold height activate this collision avoidance
    if elevator theta relative to drivebase is within range {} or {} range, then check if ball intake is currently above minimum steta
    if ball intake is below min theta, then run elevator ball intake coordination to keep ball intake at same height with a new theta (optional)
*/

int elevatorHeightDangerZone = 100; //Placeholder
int elevatorThetaDangerZone[2][2] = {{45,120},{240,315}}; //Lower, upper, lower, upper
int ballIntakeThetaDangerZone = 100;

void Superstructure::elevatorHeightBallIntakeCoordination() {
    if(desiredBallIntakeState.theta > ballIntakeThetaDangerZone) {

    }
}

void Superstructure::elevatorThetaBallIntakeCoordination() {

}