#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include "ServoMotor.h"
#include <Servo.h>

class ServoMotorImpl: public ServoMotor {
	public:
		ServoMotorImpl(uint8_t pin);
	  	void setOn() override;
	  	void setPosition(uint8_t angle) override;
	  	void setOff() override;

	private:
	  	uint8_t pin;
	  	bool on;
	  	Servo motor;
};

#endif
