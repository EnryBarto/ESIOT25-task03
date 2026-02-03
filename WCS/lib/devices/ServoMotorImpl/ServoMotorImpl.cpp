#include "ServoMotorImpl.h"
#include "Arduino.h"

ServoMotorImpl::ServoMotorImpl(uint8_t pin) {
  	this->pin = pin;
  	on = false;
}

void ServoMotorImpl::setOn() {
  	motor.attach(this->pin);
  	this->on = true;
}

void ServoMotorImpl::setPosition(uint8_t angle) {
	if(angle > 180) {
		angle = 180;
	} else if(angle < 0) {
		angle = 0;
	}
  	this->motor.write(angle);
}

void ServoMotorImpl::setOff() {
  	this->on = false;
  	this->motor.detach();
}
