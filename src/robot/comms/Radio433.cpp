#include "Radio433.h"

RH_ASK driver(1000);
uint8_t packet[6] = {0,0,0,0,0,0}; //ball theta, hatch theta, ball intake, ball outtake, hatch intake, hatch outtake


Radio433::Radio433()
{
}

Radio433::~Radio433()
{
}

void Radio433::init() {
    driver.init();
}

void Radio433::update() {
    driver.send(packet, sizeof(packet));
    driver.waitPacketSent();
}

void Radio433::sendPacket(uint8_t _packet[6]) {
    for(int i = 0; i < sizeof(_packet); i++) {
        packet[i] = _packet[i];
    }
}