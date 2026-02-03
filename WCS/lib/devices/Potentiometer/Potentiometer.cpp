#include "Potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin) : Potentiometer(pin, 0) { }

Potentiometer::Potentiometer(uint8_t pin, uint16_t tollerance) {
    this->pin = pin;
    this->tollerance = this->tollerance >= 0 ? tollerance : 0;
    pinMode(pin, INPUT);
    this->lastValue = analogRead(this->pin);
}

bool Potentiometer::isChanged() {
    return abs(this->lastValue - analogRead(this->pin)) > this->tollerance;
}

uint16_t Potentiometer::getValue() {
    this->lastValue = analogRead(this->pin);
    return this->lastValue;
}
