#include <stdint.h>
#include <robot/defs.h>

class Mechanisms
{
private:
    static Mechanisms* singleInstance;
    void setMechanismTheta(bool ballMech, int8_t theta);
public:
    static Mechanisms* Instance();
    int8_t packet[ML];
    Mechanisms(/* args */);
    ~Mechanisms();
    void update();
    void setMechanismState(bool ballMech, bool deployed);
    void setIntakeOuttake(bool ballMech, bool intake, bool outtake);
    bool previousBallState;
    bool previousHatchState;
};
