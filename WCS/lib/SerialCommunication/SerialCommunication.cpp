#include "SerialCommunication.h"

void SerialCommunication::loop() {

    if (Serial.available() > 0) {

        String msg = Serial.readStringUntil(MESSAGE_DELIMITER);
        switch (msg.charAt(0)) {
            case TOGGLE_MODE_CMD:
                this->toggleMode = true;
                break;

            case UNCONNECTED_CMD:
                this->unconnectedMode = true;
                break;

            case RESTORED_CON_CMD:
                this->restoredConnection = true;
                break;

            case SET_VALUE_CMD:
                this->arrivedValue = true;
                msg.remove(0, 2);
                this->value = msg.toInt();
                return;
        }
    }
}

bool SerialCommunication::isToggleMode() {
    bool tmp = this->toggleMode;
    if (tmp) this->resetValues();
    return tmp;
}

bool SerialCommunication::isUnconnectedMode() {
    bool tmp = this->unconnectedMode;
    if (tmp) this->resetValues();
    return tmp;
}

bool SerialCommunication::isConnectionRestored() {
    bool tmp = this->restoredConnection;
    if (tmp) this->resetValues();
    return tmp;
}

bool SerialCommunication::isValueArrived() {
    bool tmp = this->arrivedValue;
    return tmp;
}

uint8_t SerialCommunication::getArrivedValue() {
    if (this->arrivedValue) this->resetValues();
    return this->value;
}

void SerialCommunication::sendToggleMode() {
    Serial.println(TOGGLE_MODE_CMD);
}

void SerialCommunication::sendOpeningChanged(int val) {
    char buf[6];
    sprintf(buf, "%c-%d", SET_VALUE_CMD, val);
    Serial.println(buf);
}

void SerialCommunication::resetValues() {
    this->toggleMode = false;
    this->restoredConnection = false;
    this->unconnectedMode = false;
    this->arrivedValue = false;
}