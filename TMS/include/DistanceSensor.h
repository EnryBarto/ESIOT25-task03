#ifndef __DISTANCE_SENSOR__
#define __DISTANCE_SENSOR__

#include <stdint.h>

class DistanceSensor {

    public:
        virtual uint16_t getDistance() = 0;
};

#endif