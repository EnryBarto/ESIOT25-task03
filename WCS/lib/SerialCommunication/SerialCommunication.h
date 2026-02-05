#ifndef __SERIAL_COMM__
#define __SERIAL_COMM__

#include <Arduino.h>
#include "protocol_commands.h"

class SerialCommunication {

    public:
        void loop(); // Check if there are incoming messages and set the corresponding flag
        // When these methods are invoked, the corresponding flag is reset
        bool isToggleMode();
        bool isUnconnectedMode();
        bool isConnectionRestored();
        // The arrived value flag is reset only when the value is read
        bool isValueArrived();
        uint8_t getArrivedValue();
        void sendToggleMode();
        void sendOpeningChanged(int val);

    private:
        bool toggleMode = false;
        bool unconnectedMode = false;
        bool restoredConnection = false;
        bool arrivedValue = false;
        uint8_t value;
        void resetValues();
};

#endif