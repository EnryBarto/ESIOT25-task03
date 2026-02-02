#include "ConnectionTask.h"

ConnectionTask::ConnectionTask() {
    this->currState = WIFI_DISCONNECTED;
}

void ConnectionTask::init(SharedData *sharedData) {
    this->sharedData = sharedData;
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("TMS");
    this->sharedData->setMqttClient(new PubSubClient(this->wifiClient));
    this->sharedData->getMqttClient()->setServer(MQTT_SERVER, MQTT_PORT);
}

void ConnectionTask::tick() {

    switch (this->currState) {

        case CONNECTED:
            if (WiFi.status() != WL_CONNECTED) {
                this->currState = WIFI_DISCONNECTED;
            } else if (!this->sharedData->getMqttClient()->connected()) {
                this->currState = MQTT_CONNECTING;
            } else {
                this->sharedData->getMqttClient()->loop();
            }
            break;

        case MQTT_CONNECTING:
            if (WiFi.status() != WL_CONNECTED) {
                this->currState = WIFI_DISCONNECTED;
            } else if (this->sharedData->getMqttClient()->connected()) {
                this->currState = CONNECTED;
            } else if (millis() - this->lastMqttConnectionAttempt >= TIMEOUT_MQTT_RECONNECTION) {
                this->precState = NONE; // FORCE STATE TRANSITION
            }
            break;

        case WIFI_CONNECTING:
            if (WiFi.status() == WL_CONNECTED) {
                this->currState = MQTT_CONNECTING;
            } else if (millis() - this->lastWifiConnectionAttempt >= TIMEOUT_WIFI_RECONNECTION) {
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
            #ifdef DEBUG
            Serial.println("CONNECTION TASK: Attempt connecting WiFi");
            #endif
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            this->lastWifiConnectionAttempt = millis();
            this->sharedData->setWifiError(true);
            break;

        case MQTT_CONNECTING:
            #ifdef DEBUG
            Serial.println("CONNECTION TASK: Attempt connecting MQTT broker");
            #endif
            this->sharedData->getMqttClient()->connect("TMS");
            this->lastMqttConnectionAttempt = millis();
            this->sharedData->setMqttError(true);
            break;

        case WIFI_DISCONNECTED:
            #ifdef DEBUG
            Serial.println("CONNECTION TASK: WiFi disconnected");
            #endif
            WiFi.disconnect();
            this->sharedData->setWifiError(true);
            break;

        case CONNECTED:
            #ifdef DEBUG
            Serial.println("CONNECTION TASK: WiFi and MQTT connected");
            #endif
            this->sharedData->setWifiError(false);
            this->sharedData->setMqttError(false);
            break;
    }
}

const char *ConnectionTask::getName() {
    return "Connection Task";
}
