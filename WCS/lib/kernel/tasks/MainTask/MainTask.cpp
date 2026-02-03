#include "MainTask.h"
#include <Arduino.h>

MainTask::MainTask(SharedData* shared, Lcd* lcd, Button* modeButton, Slider* potentiometer, ServoMotor* motor) {
    this->currentState = IDLE;
    this->precState = IDLE; // Used to force state update action
    this->lcd = lcd;
    this->modeButton = modeButton;
    this->potentiometer = potentiometer;
    this->motor = motor;
}

void MainTask::tick() {
    // FSM actions
    switch (currentState) {
        default:
            break;
    }

    if (currentState != precState) {
        precState = currentState;
    }
}