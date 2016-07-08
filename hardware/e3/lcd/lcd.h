#ifndef LCD_H

	#include "../const.h"
	#include <LiquidCrystal_I2C.h>
	#include <LiquidCrystal.h>
	#include <LCD.h>
	#include <stdio.h>
	#include <stdlib.h>

	#define LCD_I2C_BUS 0x3F

	extern LiquidCrystal_I2C lcd;
	
	void lcdInit();
	void lcdUpdateTempHumid(float temperature, float humidity);
	void lcdUpdateDustCo(float dustLevel, float coLevel);
	void lcdWrite(char* msg);
	void lcdUpdateWifiStatus(unsigned char status);
	void lcdClear();

#endif