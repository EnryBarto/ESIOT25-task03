#ifndef __TASK__
#define __TASK__

#include "SharedData/SharedData.h"
#include <stdint.h>

class Task {

    public:
        virtual void init(SharedData* sharedData) = 0;
        virtual void tick() = 0;
        virtual void stateTransition() = 0;
        virtual const char* getName() = 0;
};

#endif