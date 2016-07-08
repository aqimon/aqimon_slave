#ifndef DUST_H

	#define DUST_H

	#include <Arduino.h>

	#define DUST_LED_PIN 2
	#define DUST_DATA_PIN A3

	void dustInit();
	void dustRead(float* voltage, float* density);

#endif