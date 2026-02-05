#include "Potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin) : Potentiometer(pin, 0) { }

Potentiometer::Potentiometer(uint8_t pin, uint16_t tollerance) {
    this->pin = pin;
    this->tollerance =  tollerance;
    pinMode(pin, INPUT);
    this->lastValue = analogRead(this->pin);
}

// Force the next isChanged to be true
void Potentiometer::start() {
    this->lastValue = INT16_MAX;
}

// The value is changed only if it's outside the tollerance range
bool Potentiometer::isChanged() {
    int16_t tmp = analogRead(this->pin);
    return (tmp < (int16_t)((int16_t)this->lastValue - this->tollerance)) || (tmp > (int16_t)(this->lastValue + this->tollerance));
}

uint8_t Potentiometer::getValue() {
    this->lastValue = analogRead(this->pin);
    return map(this->lastValue, 0, 1020, 0, 100);
}