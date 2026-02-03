#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

#include <Arduino.h>
#include "Slider.h"

class Potentiometer: public Slider {

    public:
        Potentiometer(uint8_t pin);
        Potentiometer(uint8_t pin, uint16_t tollerance);
        bool isChanged() override;
        uint8_t getValue() override;
        void start() override;

    private:
        uint8_t pin;
        uint16_t lastValue;
        uint16_t tollerance;

};

#endif