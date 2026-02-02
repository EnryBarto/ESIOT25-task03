#include <Arduino.h>
#include "HardwarePlatform.h"
#include "Scheduler/Scheduler.h"
#include "tasks/StatusTask/StatusTask.h"
#include "tasks/ConnectionTask/ConnectionTask.h"
#include "tasks/LoggerTask/LoggerTask.h"

HardwarePlatform hardware;
Scheduler scheduler;

void setup() {
    hardware.init();
    scheduler.startTask(
        new ConnectionTask(),
        CONNECTION_TASK_DELAY
    );
    scheduler.startTask(
        new StatusTask(hardware.getWorkingLed(), hardware.getAlarmLed()),
        STATUS_TASK_DELAY
    );
    scheduler.startTask(
        new LoggerTask(hardware.getSonar()),
        LOGGER_TASK_DELAY
    );
}

void loop() {
}
