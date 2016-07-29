#ifndef DHT2_H

	#define DHT2_H

	#include <Arduino.h>
	#include <DHT.h>
	#include <avr/wdt.h>

	#define DHT_PIN 5

	extern DHT dht;

	void dhtInit();
	void dhtRead(float *temperature, float *humidity);

#endif