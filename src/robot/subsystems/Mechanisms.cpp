#include "Mechanisms.h"
#include "robot/comms/Radio433.h"
#include <stdint.h>
#include <robot/defs.h>

#define bts packetBits.bits

struct Bits
{
    unsigned ballIntake_b:1, ballOuttake_b:1, hatchIntake_b:1, hatchOuttake_b:1;
};
union CBits
{
    Bits bits;
    int8_t byteC;
};

CBits packetBits;

Mechanisms* Mechanisms::singleInstance = NULL;

Mechanisms* Mechanisms::Instance() {
    if(!singleInstance) {
        singleInstance = new Mechanisms();
    }
    return singleInstance;
}


Mechanisms::Mechanisms(/* args */)
{
    //radio.init();
    packet[3] = 0;
    previousHatchState = false;
    previousBallState = false;
}

Mechanisms::~Mechanisms()
{
}

void Mechanisms::update() {
    // radio.sendPacket(packet);
    // radio.update();
    
}

void Mechanisms::setMechanismTheta(bool ballMech, int8_t theta) {
    // uint8_t ballTheta;
    // uint8_t hatchTheta;
    // bool ballEnabled;
    // bool hatchEnabled;
    Serial.println("Theta set");
    if(ballMech) {
        packet[0] = theta;
    }
    else {
        packet[1] = theta;
    }
}

void Mechanisms::setMechanismState(bool ballMech, bool deployed) {
    if(ballMech) {
        setMechanismTheta(ballMech, (int8_t)(deployed ? 0:90));
        previousBallState = deployed;
    }
    else {
        setMechanismTheta(ballMech, (int8_t)(deployed ? 0:90));
        previousHatchState = deployed;
    }
}

int8_t ballIntakeP = 0; //Each part of the byte
int8_t hatchP = 0;

void Mechanisms::setIntakeOuttake(bool ballMech, bool intake, bool outtake) {
    //Serial.println("Intake mode set");
    // if(ballMech) {
    //     if(intake) {
    //         bts.ballOuttake_b = 0;
    //         bts.ballIntake_b = 1;
    //     }
    //     else if(outtake) {
    //         bts.ballIntake_b = 0;
    //         bts.ballOuttake_b = 1;
    //     }
    // }
    // else {
    //     if(intake) {
    //         bts.hatchOuttake_b = 0;
    //         bts.hatchIntake_b = 1;
    //     }
    //     else if(outtake) {
    //         bts.hatchIntake_b = 0;
    //         bts.hatchOuttake_b = 1;
    //     }
        //TEMP DEBUG
        if(ballMech) {
            ballIntakeP = (int8_t)((intake ? 1 : 0) + (outtake ? 2 : 0));
        }
        else {
            hatchP = (int8_t)((intake ? 4 : 0) + (outtake ? 8 : 0));
        }
    // }
    packet[2] = ballIntakeP + hatchP;
    //Serial.println(packetBits.byteC);
}
