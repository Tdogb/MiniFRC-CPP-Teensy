#include <robot/motor/Brushed.h>
#include <stdint.h>

class Drivetrain
{
private:
    Drivetrain();
    Brushed* leftMotor = new Brushed(2,3);
    Brushed* rightMotor = new Brushed(4,7);  
    static Drivetrain* singleInstance;
    void pids();
public:
    static Drivetrain* Instance();
    void updateControls(int8_t throttleAxis, int8_t turningAxis);
    void updateDrivetrain();
    void debug();
    void debugScheduled();
    int8_t throttle = 0;
    int8_t turn = 0;
};