#ifndef __FOLLOWER_H__
#define __FOLLOWER_H__

#include <stdint.h>
#include "robot/motor/Brushed.h"
#include <cstddef>
#include <vector>
class Follower
{
private:
    double autoTimeStep = 10;
    uint16_t time = 0;
    size_t pathSize;
    int16_t previousSpeed = 0;
public:
    Follower(std::vector<std::vector<double>> _path, size_t _pathSize, Brushed *_motor);
    ~Follower();
    int16_t update();
};

#endif // __FOLLOWER_H__

