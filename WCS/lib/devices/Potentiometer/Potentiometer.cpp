#include "Potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin) : Potentiometer(pin, 0) { }

Potentiometer::Potentiometer(uint8_t pin, uint16_t tollerance) {
    this->pin = pin;
    this->tollerance =  tollerance;
    pinMode(pin, INPUT);
    this->lastValue = analogRead(this->pin);
}

bool Potentiometer::isChanged() {
    Serial.println(analogRead(this->pin));
    Serial.println(this->lastValue);
    int16_t tmp = analogRead(this->pin);
    return (tmp < (int16_t)((int16_t)this->lastValue - this->tollerance)) || (tmp > (int16_t)(this->lastValue + this->tollerance));
}

uint16_t Potentiometer::getValue() {
    this->lastValue = analogRead(this->pin);
    return this->lastValue;
}

void Potentiometer::start() {
    this->lastValue = UINT16_MAX;
}
