#include "SharedData.h"

SharedData::SharedData() {
}

bool SharedData::isMqttError() {
    return this->mqttError;
}

bool SharedData::isWifiError() {
    return this->wifiError;
}

void SharedData::setWifiError(bool value) {
	this->wifiError = value;
}

void SharedData::setMqttError(bool value) {
    this->mqttError = value;
}

PubSubClient *SharedData::getMqttClient() {
    return this->mqttClient;
}

void SharedData::setMqttClient(PubSubClient *client) {
    this->mqttClient = client;
}
