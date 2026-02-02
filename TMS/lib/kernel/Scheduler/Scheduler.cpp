#include "Scheduler.h"

Scheduler::Scheduler() {
    this->sharedData = new SharedData();
}

void Scheduler::startTask(Task *task, uint16_t delay) {
    if (this->numTask >= MAX_TASKS) {
        Serial.println("Max tasks reached!");
        return;
    }

    this->tasksExecutors[this->numTask] = new TaskExecutor(task, delay, this->sharedData);

    xTaskCreatePinnedToCore(
        TaskExecutor::execute,
        task->getName(),
        10000,
        this->tasksExecutors[this->numTask],
        1,
        &tasks[this->numTask],
        1
    );

    this->numTask++;
}