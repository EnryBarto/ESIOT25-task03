#include "ConnectionTask.h"

ConnectionTask::ConnectionTask() {
    this->currState = WIFI_DISCONNECTED;
}

void ConnectionTask::init(SharedData *sharedData) {
    this->sharedData = sharedData;
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("TMS");
}

void ConnectionTask::tick() {

    switch (this->currState) {

        case WIFI_CONNECTED:
            if (WiFi.status() != WL_CONNECTED) {
                this->currState = WIFI_DISCONNECTED;
            }
            break;

        case WIFI_CONNECTING:
            if (WiFi.status() == WL_CONNECTED) {
                this->currState = WIFI_CONNECTED;
            } else if (millis() - this->lastConnectionAttempt >= TIMEOUT_WIFI_RECONNECTION) {
                this->currState = WIFI_DISCONNECTED;
            }
            break;

        case WIFI_DISCONNECTED:
            this->currState = WIFI_CONNECTING;
            break;
    }

    if (this->precState != this->currState) {
        this->stateTransition();
        this->precState = this->currState;
    }
}

void ConnectionTask::stateTransition() {

    switch (this->currState) {

        case WIFI_CONNECTING:
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            this->lastConnectionAttempt = millis();
            this->sharedData->setWifiError(true);
            break;

        case WIFI_DISCONNECTED:
            WiFi.disconnect();
            this->sharedData->setWifiError(true);
            break;

        case WIFI_CONNECTED:
            this->sharedData->setWifiError(false);
            break;
    }
}

const char *ConnectionTask::getName() {
    return "Connection Task";
}
