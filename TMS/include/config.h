#ifndef __CONFIG_H
#define __CONFIG_H

#define SERIAL_SPEED 115200
// #define DEBUG YES

/* -------------------- PIN -------------------- */
#define SONAR_TRIG_PIN  33
#define SONAR_ECHO_PIN  32
#define WORKING_LED_PIN 26
#define ALARM_LED_PIN   25

/* ------------------- TASKS ------------------- */
#define CONNECTION_TASK_DELAY 500
#define STATUS_TASK_DELAY 1000
#define LOGGER_TASK_DELAY 5000

/* ------------------- MQTT ------------------- */
#define MQTT_SERVER "192.168.1.16"
#define MQTT_PORT 1883
#define TIMEOUT_MQTT_RECONNECTION 5000
#define MQTT_MSG_BUFFER_SIZE 50
#define MQTT_TOPIC "water_level"

#endif