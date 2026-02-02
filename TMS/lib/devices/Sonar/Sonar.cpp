#include "Sonar.h"

Sonar::Sonar(uint8_t echoP, uint8_t trigP) {
	this->echoPin = echoP;
	this->trigPin = trigP;
  	this->temperature = 20; // default value
  	pinMode(trigPin, OUTPUT);
  	pinMode(echoPin, INPUT);
}

void Sonar::setTemperature(float temp) {
  	this->temperature = temp;
}

float Sonar::getSoundSpeed() {
  	return 331.5 + 0.6*this->temperature;
}

uint16_t Sonar::getDistance() {
	digitalWrite(this->trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(this->trigPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(this->trigPin, LOW);

	uint16_t duration = pulseIn(this->echoPin, HIGH, MAX_TIME_OUT);

	if(duration == 0) {
		return NO_OBJ_DETECTED;
	} else {
		float t = duration / 1000.0 / 1000.0 / 2;
		float d = t*this->getSoundSpeed()*100;
		return (uint16_t)d;
	}
}

