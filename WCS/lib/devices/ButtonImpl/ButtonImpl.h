#ifndef __BUTTONIMPL__
#define __BUTTONIMPL__

#include "Button.h"
#include <Arduino.h>

class ButtonImpl: public Button {
	public:
		ButtonImpl(uint8_t pin);
		ButtonImpl(uint8_t pin, uint16_t debounceTime);
		bool isPressed() override;
		void update() override;

	private:
		uint8_t pin;
		uint64_t lastPressure = 0;
		uint64_t tick = 0;
		uint16_t debounceNumTick;
};

#endif
