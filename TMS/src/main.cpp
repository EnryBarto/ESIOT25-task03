#include <Arduino.h>
#include "HardwarePlatform.h"

TaskHandle_t task1;
TaskHandle_t task2;

HardwarePlatform hwPlat;

void Task1code(void *parameter) {
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 2000;

    for (;;) {
        Serial.println(hwPlat.getSonar()->getDistance());
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void Task2code(void *parameter) {
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 1000;

    for (;;) {
        hwPlat.getWorkingLed()->setOn();
        hwPlat.getAlarmLed()->setOff();
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        hwPlat.getWorkingLed()->setOff();
        hwPlat.getAlarmLed()->setOn();
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


void setup() {
    hwPlat.init();
    xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &task1, 0);
    xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &task2, 0);
}

void loop() {
}
