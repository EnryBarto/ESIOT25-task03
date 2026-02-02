#ifndef __CONFIG_H
#define __CONFIG_H

#define SERIAL_SPEED 115200

/* -------------------- PIN -------------------- */
#define SONAR_TRIG_PIN  33
#define SONAR_ECHO_PIN  32
#define WORKING_LED_PIN 26
#define ALARM_LED_PIN   25

/* ------------------- TASKS ------------------- */
#define CONNECTION_TASK_DELAY 500
#define STATUS_TASK_DELAY 2500
#define LOGGER_TASK_DELAY 5000

#endif