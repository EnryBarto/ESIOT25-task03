#ifndef __CONNECTION_TASK__
#define __CONNECTION_TASK__

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Task.h"
#include "config.h"
#include "wifi_config.h"
#include "SharedData/SharedData.h"

class ConnectionTask : public Task {

    public:
        ConnectionTask();
        void init(SharedData *sharedData) override;
        void tick() override;
        void stateTransition() override;
        const char* getName() override;

    private:
        SharedData *sharedData;
        enum states {CONNECTED, WIFI_CONNECTING, DISCONNECTED, MQTT_CONNECTING, NONE};
        states currState;
        states precState;
        uint32_t lastWifiConnectionAttempt, lastMqttConnectionAttempt;
        WiFiClient wifiClient;
};

#endif