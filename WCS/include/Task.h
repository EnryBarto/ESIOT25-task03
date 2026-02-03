#ifndef __TASK__
#define __TASK__

#include "SharedData/SharedData.h"
#include <stdint.h>
#include "config.h"

class Task {

    public:
        virtual void init(uint16_t period) {
            this->myPeriod = period;
            this->timeElapsed = 0;
        }

        virtual void tick() = 0;

        bool updateAndCheckTime(uint8_t basePeriod) {
            this->timeElapsed += basePeriod;
            if (this->timeElapsed >= this->myPeriod) {
                this->timeElapsed = 0;
                return true;
            } else {
                return false;
            }
        }

    protected:
        uint16_t myPeriod;
        uint16_t timeElapsed;
        SharedData* sharedData;

};

#endif
