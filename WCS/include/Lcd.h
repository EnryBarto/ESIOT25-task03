#ifndef __LCD__
#define __LCD__

#include <stdint.h>

class Lcd {

    public:
        virtual void print(const char* message, uint8_t row, uint8_t col) = 0;
        virtual void clear() = 0;
        virtual void clearRow(uint8_t row) = 0;
};

#endif