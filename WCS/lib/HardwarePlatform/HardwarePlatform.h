#ifndef __HW_PLAT__
#define __HW_PLAT__

#include "config.h"
#include "ButtonImpl/ButtonImpl.h"
#include "ServoMotorImpl/ServoMotorImpl.h"
#include "Potentiometer/Potentiometer.h"
#include "LcdI2C/LcdI2C.h"

class HardwarePlatform {
	public:
	  	HardwarePlatform();
	  	void init();
	  	Button* getModeButton();
	  	ServoMotor* getMotor();
	  	Lcd* getLcd();
		Slider* getSlider();

	private:
	  	Button* modeButton;
	  	ServoMotor* motor;
	  	Lcd* lcd;
		Slider* slider;
};

#endif
