#include "HardwarePlatform.h"
#include <Arduino.h>

HardwarePlatform::HardwarePlatform() { }

void HardwarePlatform::init() {
	#ifdef DEBUG
	Serial.begin(SERIAL_SPEED);
	#endif
  	alarmLed = new LedImpl(ALARM_LED_PIN);
  	workingLed = new LedImpl(WORKING_LED_PIN);
	distanceSensor = new Sonar(SONAR_ECHO_PIN, SONAR_TRIG_PIN);
}

Led* HardwarePlatform::getAlarmLed() {
  	return this->alarmLed;
}

Led* HardwarePlatform::getWorkingLed() {
	return this->workingLed;
}

DistanceSensor* HardwarePlatform::getSonar() {
  	return this->distanceSensor;
}