#include "ButtonImpl.h"
#include "Arduino.h"

ButtonImpl::ButtonImpl(uint8_t pin) : ButtonImpl(pin, 0) {
}

ButtonImpl::ButtonImpl(uint8_t pin, uint16_t debounceTick) {
	this->pin = pin;
	this->debounceNumTick = debounceTick;
	pinMode(pin, INPUT);
}

bool ButtonImpl::isPressed() {
	if (this->lastPressure != 0 && this->tick - this->lastPressure < this->debounceNumTick) {
		return false;
	}

	if (digitalRead(this->pin)) {
		this->lastPressure = this->tick;
		return true;
	}

	return false;
}

void ButtonImpl::update() {
	if (this->tick == UINT64_MAX) {
		lastPressure = 0;
	}
	this->tick++;
}