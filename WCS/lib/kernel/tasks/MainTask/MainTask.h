#ifndef __MAIN_TASK__
#define __MAIN_TASK__

#include "Task.h"
#include "SharedData/SharedData.h"
#include "Lcd.h"
#include "Button.h"
#include "Slider.h"
#include "ServoMotor.h"
#include "protocol_commands.h"
#include "SerialCommunication.h"

class MainTask : public Task {

    public:
        MainTask(SharedData* shared, Lcd* lcd, Button* modeButton, Slider* potentiometer, ServoMotor* motor);
        void tick() override;

    private:
        enum states {AUTOMATIC, MANUAL, UNCONNECTED_AUTO, UNCONNECTED_MAN};
        states currentState;
        states precState;
        enum manual_modes {PHYSICAL, VIRTUAL};
        manual_modes currentManualMode;
        Lcd* lcd;
        Button* modeButton;
        Slider* potentiometer;
        ServoMotor* motor;
        SerialCommunication serialCom;
        void setOpening(uint8_t percentage);
};

#endif