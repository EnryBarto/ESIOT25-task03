#include "LoggerTask.h"

LoggerTask::LoggerTask(DistanceSensor *sensor) {
    this->sensor = sensor;
    this->currState = NOT_DETECTING;
}

void LoggerTask::init(SharedData *sharedData) {
    this->sharedData = sharedData;
}

void LoggerTask::tick() {
    switch (this->currState) {
        case NOT_DETECTING:
            if (!this->sharedData->isWifiError() && !this->sharedData->isMqttError()) {
                this->currState = DETECTION;
            }
            break;

        case DETECTION:
            if (this->sharedData->isWifiError() || this->sharedData->isMqttError()) {
                this->currState = NOT_DETECTING;
            } else {
                this->distance = this->sensor->getDistance();
                snprintf(this->msgBuffer, MQTT_MSG_BUFFER_SIZE, "%ld", this->distance == NO_OBJ_DETECTED ? -1 : this->distance);
                bool publishStatus = this->sharedData->getMqttClient()->publish(MQTT_TOPIC, this->msgBuffer);
                this->sharedData->setMqttError(!publishStatus);
                #ifdef DEBUG
                Serial.printf("LOGGER TASK: Distance: %d\n", this->distance);
                Serial.printf("LOGGER TASK: Message published status %s\n", publishStatus ? "OK" : "Error");
                #endif
            }
            break;
    }
}

void LoggerTask::stateTransition() {
}

const char *LoggerTask::getName() {
    return "Logger Task";
}
