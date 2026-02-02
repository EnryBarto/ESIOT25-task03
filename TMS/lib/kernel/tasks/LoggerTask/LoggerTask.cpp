#include "LoggerTask.h"

LoggerTask::LoggerTask(DistanceSensor *sensor) {
    this->sensor = sensor;
    this->currState = DETECTION;
}

void LoggerTask::init(SharedData *sharedData) {
    this->sharedData = sharedData;
}

void LoggerTask::tick() {
    switch (this->currState) {
        case DETECTION:
            uint16_t distance = this->sensor->getDistance();
            break;
    }
}

void LoggerTask::stateTransition() {
}

const char *LoggerTask::getName() {
    return "Logger Task";
}
