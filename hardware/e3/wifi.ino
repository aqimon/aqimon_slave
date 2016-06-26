#include <SoftwareSerial.h>
#define WIFI_TX 3
#define WIFI_RX 4

SoftwareSerial wifi(WIFI_RX, WIFI_TX);

void wifiInit(){
	wifi.begin(115200);
}

void wifiLoop(){
	if (wifi.available()) {
    	Serial.print(wifi.read());
  	}	
  	if (Serial.available()) {
    	wifi.write(Serial.read());
  	}
}
