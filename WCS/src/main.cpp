#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "Scheduler/Scheduler.h"
#include "tasks/MainTask/MainTask.h"
#include "HardwarePlatform.h"
#include "SharedData/SharedData.h"
#include <avr/io.h>

#define BASE_PERIOD 50
#define MAIN_TASK_PERIOD 500

Scheduler sched;
HardwarePlatform hwPlat;
SharedData* sharedData;

volatile uint8_t interruptCounter = 0;
volatile bool wakeUpFlag = false;

// Timer2 interrupt service routine
ISR(TIMER2_COMPA_vect) {
    interruptCounter++;
    if (interruptCounter >= 5) {  // 5 * ~10ms = ~50ms
        interruptCounter = 0;
        wakeUpFlag = true;
    }
}

void setupTimer2();

void setup() {
    delay(500); // Used to sync serial communication

    setupTimer2();

    sched.init(BASE_PERIOD);
    hwPlat.init();
    sharedData = new SharedData();

    MainTask* mainTask = new MainTask(
        sharedData,
        hwPlat.getLcd(),
        hwPlat.getModeButton(),
        hwPlat.getSlider(),
        hwPlat.getMotor()
    );
    mainTask->init(MAIN_TASK_PERIOD);
    sched.addTask(mainTask);
}

void loop() {
    if (wakeUpFlag) {
        wakeUpFlag = false;
        sched.schedule();
    }
    // This prevents the ~1ms Timer0 wakeups while we're using Timer2.
    // This disables millis()/delay() while in sleep.
    uint8_t oldTIMSK0 = TIMSK0;
    TIMSK0 &= ~(1 << TOIE0);

    // Idle is the only sleep mode that keeps usart line open to receive.
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
    sleep_mode();
    sleep_disable();

    // restore Timer0
    TIMSK0 = oldTIMSK0;
}

void setupTimer2() {
    cli();
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = 156;
    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
    TIMSK2 |= (1 << OCIE2A);
    sei();
}
