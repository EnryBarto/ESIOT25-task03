#ifndef __SLIDER__
#define __SLIDER__

#include <stdint.h>

class Slider {

    public:
        virtual bool isChanged() = 0;
        virtual uint8_t getValue() = 0; // Returns a value in percentage [0, 100]
        virtual void start() = 0;

};


#endif