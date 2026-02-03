#include "LcdI2C.h"
#include <Arduino.h>

LcdI2C::LcdI2C(uint8_t address, uint8_t cols, uint8_t rows) {
    lcd = new LiquidCrystal_I2C(address, cols, rows);
    lcd->init();
    lcd->backlight();
    this->cols = cols;
}

void LcdI2C::print(const char* message, uint8_t row, uint8_t col) {
    lcd->setCursor(col, row);
    lcd->print(message);
}

void LcdI2C::clear() {
    lcd->clear();
}

void LcdI2C::clearRow(uint8_t row) {
    lcd->setCursor(0, row);
    for (uint8_t i = 0; i < this->cols; i++) {
        lcd->print(" ");
    }
}