#include "HardwarePlatform.h"
#include <Arduino.h>

HardwarePlatform::HardwarePlatform() {
}

void HardwarePlatform::init() {
  	modeButton = new ButtonImpl(MODE_BTN_PIN);
  	motor = new ServoMotorImpl(MOTOR_PIN);
  	lcd = new LcdI2C(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);
	slider = new Potentiometer(POTENTIOMETER_PIN, POTENTIOMETER_TOLLERANCE);
	Serial.begin(SERIAL_SPEED);
}

Button* HardwarePlatform::getModeButton() {
  	return this->modeButton;
}

ServoMotor* HardwarePlatform::getMotor() {
  	return this->motor;
}

Lcd* HardwarePlatform::getLcd() {
  	return this->lcd;
}

Slider *HardwarePlatform::getSlider() {
    return this->slider;
}
