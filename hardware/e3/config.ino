/*#include <avr/eeprom.h>
#define CONFIG_EEPROM_ADDRESS 0

typedef unsigned char str[50];
typedef unsigned char ipAddr[4];
struct staticIP_s {
	ipAddr ip;
	ipAddr subnetMask;
	ipAddr defaultGateway;
};

struct wifiSettings_s {
	str ssid;
	str password;
};

struct config_s {
	struct staticIP_s etherIP;
	struct staticIP_s wifiIP;
	struct wifiSettings_s wifiSettings;
	ipAddr dns;
	str hostname;
	str apiKey;
} config;

void configRead(){
	eeprom_read_block(&config, CONFIG_EEPROM_ADDRESS, sizeof(config));
}

void configWrite(){
	eeprom_update_block(&config, CONFIG_EEPROM_ADDRESS, sizeof(config));
}*/



