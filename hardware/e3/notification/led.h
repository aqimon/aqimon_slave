#ifndef LED_H
	
	#define LED_H

	#include "../const.h"
	#include <Arduino.h>

	#define LED_DS 6 // data
	#define LED_STCP 7 // latch
	#define LED_SHCP 10 // clock

	void ledInit();
	void ledSetLeft(const char color);
	void ledSetRight(const char color);
	void ledSetColor();

#endif