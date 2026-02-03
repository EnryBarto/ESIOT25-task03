#ifndef __SERVO_MOTOR__
#define __SERVO_MOTOR__

#include <stdint.h>

class ServoMotor {

    public:
        virtual void setOn() = 0;
        virtual void setPosition(uint8_t angle) = 0;
        virtual void setOff() = 0;
};

#endif
