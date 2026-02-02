#include "LedImpl.h"
#include "Arduino.h"

LedImpl::LedImpl(uint8_t pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

void LedImpl::setOn() {
    digitalWrite(this->pin, HIGH);
}

void LedImpl::setOff() {
    digitalWrite(this->pin, LOW);
}