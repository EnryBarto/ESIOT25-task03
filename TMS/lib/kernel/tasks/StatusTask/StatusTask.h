#ifndef __STATUS_TASK__
#define __STATUS_TASK__

#include <Arduino.h>
#include "Task.h"
#include "SharedData/SharedData.h"
#include "Led.h"

class StatusTask : public Task {

    public:
        StatusTask(Led *workingLed, Led *alarmLed);
        void init(SharedData *sharedData) override;
        void tick() override;
        void stateTransition() override;
        const char* getName() override;

    private:
        SharedData *sharedData;
        enum states {OK, ALARM, NONE};
        states currState;
        states precState;
        Led *workingLed, *alarmLed;
};

#endif