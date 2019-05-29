#include <Arduino.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
#define RH_HAVE_SERIAL

//RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 

class Radio433
{
private:
public:
    Radio433(/* args */);
    ~Radio433();
    void init();
    void update();
    void sendPacket(uint8_t _packet[6]);

};
