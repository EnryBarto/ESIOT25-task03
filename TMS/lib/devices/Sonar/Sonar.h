#ifndef __SONAR__
#define __SONAR__

#include "DistanceSensor.h"
#include <Arduino.h>

#define NO_OBJ_DETECTED __UINT16_MAX__
#define MAX_TIME_OUT 6000

class Sonar : public DistanceSensor {
	public:
	  	Sonar(uint8_t echoPin, uint8_t trigPin);
	  	uint16_t getDistance() override;
	  	void setTemperature(float temp);

	private:
		const float vs = 331.5 + 0.6*20;
		float getSoundSpeed();
		float temperature;
		uint8_t echoPin, trigPin;
};

#endif
