#include "MainTask.h"
#include <Arduino.h>

MainTask::MainTask(SharedData* shared, Lcd* lcd, Button* modeButton, Slider* potentiometer, ServoMotor* motor) {
    this->currentState = AUTOMATIC;
    this->precState = MANUAL; // Used to force state update action
    this->lcd = lcd;
    this->modeButton = modeButton;
    this->potentiometer = potentiometer;
    this->motor = motor;
}

void MainTask::tick() {
    this->modeButton->update();

    // FSM actions
    switch (currentState) {
        case AUTOMATIC:
            if (this->modeButton->isPressed()) {
                this->currentState = MANUAL;
            }
            break;

        case MANUAL:
            if (this->modeButton->isPressed()) {
                this->currentState = AUTOMATIC;
            } else if (this->potentiometer->isChanged()) {
                this->currentManualMode = PHYSICAL;
            }

            switch (this->currentManualMode) {
                case PHYSICAL:
                    if (this->potentiometer->isChanged()) {
                        lcd->clearRow(VALUE_LCD_ROW);
                        char buf[5];
                        sprintf(buf, "%d", this->potentiometer->getValue());
                        lcd->print(buf, VALUE_LCD_ROW, 0);
                    }
                    break;

                case VIRTUAL:
                    break;
            }
            break;

    }

    if (currentState != precState) {
        switch (this->currentState) {
            case AUTOMATIC:
                lcd->clearRow(MODE_LCD_ROW);
                lcd->clearRow(VALUE_LCD_ROW);
                lcd->print("Mode: AUTOMATIC", MODE_LCD_ROW, 0);
                break;

            case MANUAL:
                this->potentiometer->start();
                this->currentManualMode = PHYSICAL;
                lcd->clearRow(MODE_LCD_ROW);
                lcd->print("Mode: MANUAL", MODE_LCD_ROW, 0);
                break;
        }
        precState = currentState;
    }
}