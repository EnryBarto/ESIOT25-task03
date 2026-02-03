#ifndef __BUTTON__
#define __BUTTON__

class Button {

    public:
        virtual bool isPressed() = 0;
        virtual void update() = 0;      // Call in the loop to count the ticks
};

#endif
