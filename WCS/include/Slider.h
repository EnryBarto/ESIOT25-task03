#ifndef __SLIDER__
#define __SLIDER__

#include <stdint.h>

/* Interface to model a generic analogic input method */
class Slider {

    public:
        virtual void start() = 0;       // Make the next isChanged return true
        virtual bool isChanged() = 0;   // Tell if the value is the same as the old read
        virtual uint8_t getValue() = 0; // Returns a value in percentage [0, 100]

};


#endif