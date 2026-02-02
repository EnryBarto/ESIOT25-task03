#ifndef __SCHEDULER__
#define __SCHEDULER__

#include <Arduino.h>

#include "Task.h"
#include "TaskExecutor/TaskExecutor.h"

#define MAX_TASKS 50

class Scheduler {

    public:
        Scheduler();
        void startTask(Task *task, uint16_t delay);

    private:
        SharedData *sharedData;
        uint8_t numTask = 0;
        TaskHandle_t tasks[MAX_TASKS];
        TaskExecutor *tasksExecutors[MAX_TASKS];
};

#endif