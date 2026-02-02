#ifndef __LED_IMPL__
#define __LED_IMPL__

#include "Led.h"
#include <Arduino.h>

class LedImpl: public Led {
    public:
        LedImpl(uint8_t pin);
        void setOn() override;
        void setOff() override;

    private:
        uint8_t pin;
};

#endif