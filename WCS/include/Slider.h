#ifndef __SLIDER__
#define __SLIDER__

#include <stdint.h>

class Slider {

    public:
        virtual bool isChanged() = 0;
        virtual uint16_t getValue() = 0;
        virtual void start() = 0;

};


#endif