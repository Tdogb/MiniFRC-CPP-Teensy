#include <stdint.h>

class Mechanisms
{
private:
    static Mechanisms* singleInstance;
    void setMechanismTheta(bool ballMech, uint8_t theta);
    uint8_t packet[6] = {0,0,0,0,0,0};
public:
    static Mechanisms* Instance();
    Mechanisms(/* args */);
    ~Mechanisms();
    void update();
    void setMechanismState(bool ballMech, bool deployed);
    void setIntakeOuttake(bool ballMech, bool intake, bool outtake);
    bool previousBallState = false;
    bool previousHatchState = false;
};
