#ifndef __CONFIG__
#define __CONFIG__

// GPIO Used
#define MODE_BTN_PIN 2
#define MOTOR_PIN 3
#define POTENTIOMETER_PIN A0

// Communication configs
#define LCD_I2C_ADDRESS 0x27
#define SERIAL_SPEED 9600

// Hardware configs
#define POTENTIOMETER_TOLLERANCE 15
#define BUTTON_NUM_TICK_DEBOUNCE 4
#define MAX_SERVO_OPENING 90

// Lcd parameters
#define MODE_LCD_ROW 0
#define VALUE_LCD_ROW 2
#define LCD_COLS 20
#define LCD_ROWS 4

// Task configs
#define BASE_PERIOD 50
#define MAIN_TASK_PERIOD 250


#endif
