#ifndef __LCDI2C__
#define __LCDI2C__

#include "Lcd.h"
#include "LiquidCrystal_I2C.h"

class LcdI2C : public Lcd {
	public:
	  	LcdI2C(uint8_t address, uint8_t cols, uint8_t rows);
		void print(const char* message, uint8_t row, uint8_t col) override;
	  	void clear() override;
		void clearRow(uint8_t row) override;

	private:
	  	LiquidCrystal_I2C* lcd;
		uint8_t cols;
};

#endif