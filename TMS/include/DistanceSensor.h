#ifndef __DISTANCE_SENSOR__
#define __DISTANCE_SENSOR__

#include <stdint.h>

#define NO_OBJ_DETECTED UINT16_MAX

class DistanceSensor {

    public:
        virtual uint16_t getDistance() = 0;
};

#endif