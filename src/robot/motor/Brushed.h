#include <stdint.h>

class Brushed
{
private:
    int FWDPin;
    int RVSPin;
    int rampTime;
    bool enPinMode;
    int enPin;
public:
    Brushed(int _FWDPin, int _RVSPin, int _enPin, bool _enPinMode);
    void rotate(int speed);
    void ramp();
};
