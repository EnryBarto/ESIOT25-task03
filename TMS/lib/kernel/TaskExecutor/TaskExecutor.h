#ifndef __TASK_EXECUTOR__
#define __TASK_EXECUTOR__

#include "Task.h"
#include <Arduino.h>

class TaskExecutor {

    public:
        TaskExecutor(Task *toExecute, uint32_t delay, SharedData *sharedData);
        static void execute(void *params);

    private:
        SharedData *sharedData;
        Task *toExecute;
        uint32_t delay;

};

#endif