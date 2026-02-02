#include "StatusTask.h"

StatusTask::StatusTask(Led *workingLed, Led *alarmLed) {
    this->workingLed = workingLed;
    this->alarmLed = alarmLed;
    this->currState = OK;
    this->precState = NONE; // FORCE STATE TRANSITION
}

void StatusTask::init(SharedData *sharedData) {
    this->sharedData = sharedData;
}

void StatusTask::tick() {
    // FSM actions
    switch (currState) {
        case OK:
            if (this->sharedData->isMqttError() || this->sharedData->isWifiError()) {
                this->currState = ALARM;
            }
            break;

        case ALARM:
            if (!this->sharedData->isMqttError() && !this->sharedData->isWifiError()) {
                this->currState = OK;
            }
            break;

        case NONE:
            break;
    }

    if (currState != precState) {
        this->stateTransition();
        precState = currState;
    }
}

void StatusTask::stateTransition() {
    switch (this->currState) {
        case OK:
            this->alarmLed->setOff();
            this->workingLed->setOn();
            break;

        case ALARM:
            this->alarmLed->setOn();
            this->workingLed->setOff();
            break;
    }

}

const char* StatusTask::getName() {
    return "Status Task";
}
