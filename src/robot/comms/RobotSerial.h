#include <stdint.h>

class RobotSerial
{
private:
    unsigned long lastControlPacket = -10000;
    int8_t packet[8] = {0,0,0,0,0,0,0,0};
    bool disabled = false;
    int throttle = 0;
    int turn = 0;
    double mapDouble(double x, double in_min, double in_max, double out_min, double out_max);
    RobotSerial();
    static RobotSerial* singleInstance;
public:
    static RobotSerial* Instance();
    void update();
    void writeBT(int8_t _message[]);
    // bool squareButton = false;
    // bool xButton = false;
    // bool oButton = false;
    // bool triangleButton = false;
    bool newData = false;
    bool buttons[12];
    int8_t xLAxis = 0;
    int8_t yLAxis = 0;
    int8_t xRAxis = 0;
    int8_t yRAxis = 0;
    // double byteToDouble(uint8_t b);
};