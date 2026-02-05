#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

#include <Arduino.h>
#include "Slider.h"

class Potentiometer: public Slider {

    public:
        // The tollerance can be used to consider unchanged the read if it falls within a certain range
        Potentiometer(uint8_t pin, uint16_t tollerance);
        Potentiometer(uint8_t pin); // Consider zero tollerance
        bool isChanged() override;
        uint8_t getValue() override;
        void start() override;

    private:
        uint8_t pin;
        uint16_t lastValue;
        uint16_t tollerance;

};

#endif