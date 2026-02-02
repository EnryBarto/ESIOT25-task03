#include "TaskExecutor.h"

TaskExecutor::TaskExecutor(Task *toExec, uint32_t delay, SharedData *sharedData) {
    this->toExecute = toExec;
    this->delay = delay;
    this->sharedData = sharedData;
}

void TaskExecutor::execute(void *params) {
    TaskExecutor *executor = (TaskExecutor *)params;

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = executor->delay;

    executor->toExecute->init(executor->sharedData);

    for ( ; ; ) {
        executor->toExecute->tick();
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}