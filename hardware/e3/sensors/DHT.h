#ifndef DHT2_H

	#define DHT2_H

	#include <Arduino.h>
	#include <DHT.h>

	#define DHT_PIN 4

	extern DHT dht;

	void dhtInit();
	void dhtRead(float* temperature, float* humidity);

#endif