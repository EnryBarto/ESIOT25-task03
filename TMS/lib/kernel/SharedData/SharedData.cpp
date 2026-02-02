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
