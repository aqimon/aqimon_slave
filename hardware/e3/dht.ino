#include "DHT.h"
#define DHT_PIN 10

DHT dht(DHT_PIN, DHT21);

void dhtInit(){
	dht.begin
}

void dhtRead(float* temperature, float* humidity){
	*temperature=dht21.readTemperature();
	*humidity=dht21.readHumidity();
	if (*temperature!=*temperature)
		*temperature=30;
	if (*humidity!=*humidity)
		*humidity=30;
}
