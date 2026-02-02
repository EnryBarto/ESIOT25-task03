#ifndef __HW_PLAT__
#define __HW_PLAT__

#include "config.h"
#include "LedImpl/LedImpl.h"
#include "Sonar/Sonar.h"

class HardwarePlatform {

	public:
	  	HardwarePlatform();
	  	void init();
		Led* getWorkingLed();
	  	Led* getAlarmLed();
		DistanceSensor* getSonar();

	private:
	  	Led* workingLed;
	  	Led* alarmLed;
		DistanceSensor* distanceSensor;
};

#endif
