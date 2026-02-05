#ifndef __SERIAL_COMM__
#define __SERIAL_COMM__

#include <Arduino.h>
#include "protocol_commands.h"

class SerialCommunication {

    public:
        void loop();
        bool isToggleMode();
        bool isUnconnectedMode();
        bool isConnectionRestored();
        bool isValueArrived();
        uint8_t arrivedValue();
        void sendToggleMode();
        void sendOpeningChanged(int val);

    private:
        bool toggleMode = false;
        bool unconnectedMode = false;
        bool restoredConnection = false;
        bool _arrivedValue = false;
        uint8_t value;
        void resetValues();

};

#endif