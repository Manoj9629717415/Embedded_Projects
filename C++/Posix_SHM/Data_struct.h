#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <cstdint>

struct SensorData
{
    int speed;
    double steeringAngle;
    uint64_t timestamp;
};



#endif