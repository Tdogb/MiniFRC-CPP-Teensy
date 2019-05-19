#include <stdint.h>

class Brushed
{
private:
    int FWDPin;
    int RVSPin;
    int rampTime;
public:
    Brushed(int _FWDPin, int _RVSPin);
    ~Brushed();
    void rotate(int speed);
    void ramp();
};
