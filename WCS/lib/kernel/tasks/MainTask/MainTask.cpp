#include "MainTask.h"
#include <Arduino.h>

MainTask::MainTask(SharedData* shared, Lcd* lcd, Button* modeButton, Slider* potentiometer, ServoMotor* motor) {
    this->currentState = AUTOMATIC;
    this->precState = MANUAL; // Used to force state update action
    this->lcd = lcd;
    this->modeButton = modeButton;
    this->potentiometer = potentiometer;
    this->motor = motor;
    this->motor->setOn();
    this->motor->setPosition(0);
}

void MainTask::tick() {
    this->modeButton->update();
    this->serialCom.loop();

    // FSM actions
    switch (currentState) {
        case AUTOMATIC:
            if (this->modeButton->isPressed() || this->serialCom.isToggleMode()) {
                this->currentState = MANUAL;
                this->serialCom.sendToggleMode();
            } else if (this->serialCom.isUnconnectedMode()) {
                this->currentState = UNCONNECTED_AUTO;
            } else if (this->serialCom.isValueArrived()) {
                setOpening(this->serialCom.getArrivedValue());
            }
            break;

        case MANUAL:
            if (this->modeButton->isPressed() || this->serialCom.isToggleMode()) {
                this->currentState = AUTOMATIC;
                this->serialCom.sendToggleMode();
            } else if (this->potentiometer->isChanged()) {
                this->currentManualMode = PHYSICAL;
            } else if (this->serialCom.isValueArrived()) {
                this->currentManualMode = VIRTUAL;
            } else if (this->serialCom.isUnconnectedMode()) {
                this->currentState = UNCONNECTED_MAN;
            }

            // Internal FSM when in MANUAL mode
            switch (this->currentManualMode) {
                case PHYSICAL:
                    if (this->potentiometer->isChanged()) {
                        uint8_t value = this->potentiometer->getValue();
                        setOpening(value);
                        this->serialCom.sendOpeningChanged(value);
                    }
                    break;

                case VIRTUAL:
                    if (this->serialCom.isValueArrived()) {
                        uint8_t value = this->serialCom.getArrivedValue();
                        setOpening(value);
                        this->serialCom.sendOpeningChanged(value);
                    }
                    break;
            }
            break;

        case UNCONNECTED_AUTO:
            if (this->serialCom.isConnectionRestored()) {
                this->currentState = AUTOMATIC;
            }
            break;

        case UNCONNECTED_MAN:
            if (this->serialCom.isConnectionRestored()) {
                this->currentState = MANUAL;
            }
            break;

    }

    if (currentState != precState) {
        switch (this->currentState) {
            case AUTOMATIC:
                lcd->clearRow(MODE_LCD_ROW);
                lcd->print("Mode: AUTOMATIC", MODE_LCD_ROW, 0);
                lcd->clearRow(VALUE_LCD_ROW);
                lcd->print("Opening:", VALUE_LCD_ROW, 0);
                break;

            case MANUAL:
                this->potentiometer->start();
                this->currentManualMode = PHYSICAL;
                lcd->clearRow(MODE_LCD_ROW);
                lcd->print("Mode: MANUAL", MODE_LCD_ROW, 0);
                lcd->clearRow(VALUE_LCD_ROW);
                lcd->print("Opening:", VALUE_LCD_ROW, 0);
                break;

            case UNCONNECTED_AUTO:
            case UNCONNECTED_MAN:
                lcd->clearRow(MODE_LCD_ROW);
                lcd->clearRow(VALUE_LCD_ROW);
                lcd->print("Mode: UNCONNECTED", MODE_LCD_ROW, 0);
                break;
        }
        precState = currentState;
    }
}

void MainTask::setOpening(uint8_t percentage) {
    if (percentage > 100) percentage = 100;
    char buf[LCD_COLS];
    sprintf(buf, "%3d%%", percentage);
    lcd->print(buf, VALUE_LCD_ROW, 9);
    this->motor->setPosition(percentage * MAX_SERVO_OPENING / 100);
}