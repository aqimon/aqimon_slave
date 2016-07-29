#include "DHT.h"

DHT dht(DHT_PIN, DHT21);

void dhtInit() {
    dht.begin();
    unsigned long t=millis();
    while (millis()-t<=2000)
    	wdt_reset();
}

void dhtRead(float *temperature, float *humidity) {
    *temperature = dht.readTemperature();
    *humidity = dht.readHumidity();
    if (*temperature != *temperature)
        *temperature = 30;
    if (*humidity != *humidity)
        *humidity = 30;
}
