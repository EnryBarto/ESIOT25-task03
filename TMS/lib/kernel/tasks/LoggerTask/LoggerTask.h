#ifndef __LOGGER_TASK__
#define __LOGGER_TASK__

#include <Arduino.h>
#include "Task.h"
#include "SharedData/SharedData.h"
#include "DistanceSensor.h"

class LoggerTask : public Task {

    public:
        LoggerTask(DistanceSensor *sensor);
        void init(SharedData *sharedData) override;
        void tick() override;
        void stateTransition() override;
        const char* getName() override;

    private:
        SharedData *sharedData;
        enum states {DETECTION};
        states currState;
        DistanceSensor *sensor;
};

#endif