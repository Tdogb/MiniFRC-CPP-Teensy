#include "Mechanisms.h"
#include "robot/comms/Radio433.h"

Radio433 radio;

Mechanisms* Mechanisms::singleInstance = NULL;

Mechanisms* Mechanisms::Instance() {
    if(!singleInstance) {
        singleInstance = new Mechanisms();
    }
    return singleInstance;
}

Mechanisms::Mechanisms(/* args */)
{
    radio.init();
}

Mechanisms::~Mechanisms()
{
}

void Mechanisms::update() {
    radio.sendPacket(packet);
    radio.update();
}

void Mechanisms::setMechanismTheta(bool ballMech, uint8_t theta) {
    // uint8_t ballTheta;
    // uint8_t hatchTheta;
    // bool ballEnabled;
    // bool hatchEnabled;
    if(ballMech) {
        packet[0] = theta;
    }
    else {
        packet[1] = theta;
    }
}

void Mechanisms::setMechanismState(bool ballMech, bool deployed) {
    if(ballMech) {
        setMechanismTheta(ballMech, (uint8_t)(deployed ? 100:150));
        previousBallState = deployed;
    }
    else {
        setMechanismTheta(ballMech, (uint8_t)(deployed ? 100:190));
        previousHatchState = deployed;
    }
}

void Mechanisms::setIntakeOuttake(bool ballMech, bool intake, bool outtake) {
    if(ballMech) {
        if(intake) {
            packet[2] = 255;
            packet[3] = 0;
        }
        else if(outtake) {
            packet[2] = 0;
            packet[3] = 255;
        }
    }
    else {
        if(intake) {
            packet[4] = 255;
            packet[5] = 0;
        }
        else if(outtake) {
            packet[4] = 0;
            packet[5] = 255;
        }
    }
}
