#ifndef __LED__
#define __LED__

class Led {

    public:
        virtual void setOn() = 0;
        virtual void setOff() = 0;
};

#endif