#include "config.h"

struct config_s config;
volatile unsigned long interruptTriggerTime;
volatile unsigned char configEnabled=0, configStarted=0, prevPinMode=1;

void configRead() {
    eeprom_read_block((void*)&config, (void*)0, sizeof(config));
}

void configWrite() {
    eeprom_update_block((void*)&config, (void*)0, sizeof(config));
}

void fillEEPROM() {
    int i;
    for (i = 0; i < 1024; i++)
        eeprom_write_byte((uint8_t*)i, 0);
    configRead();
    config.port=80;
    configWrite();
}

int configParseIP(char *str, unsigned char *ip){
	return (sscanf(str, "%hhu.%hhu.%hhu.%hhu", &(ip[0]), &(ip[1]), &(ip[2]), &(ip[3]))==4);
}

int configParseStrn(char *src, char* dst, size_t n){
	if (strlen(src)>n)
		return 0;
	strcpy(dst, src);
	return 1;
}

int configParseStrne(char *src, char *dst, size_t n){
	if (strlen(src)!=n)
		return 0;
	strcpy(dst, src);
	return 1;
}

int configParseRequest(char *req){
	if (req[1]!='='){
		return 0;
	}
	switch (req[0]){
		case 'i': // ip
			return configParseIP(req+2, config.ip);
		case 's': // subnet
			return configParseIP(req+2, config.subnetMask);
		case 'g': // gateway
			return configParseIP(req+2, config.gateway);
		case 'd': // ssid name
			return configParseStrn(req+2, config.ssidName, 31);
		case 'p': // password
			return configParseStrn(req+2, config.ssidPassword, 63);
		case 'h':
			return configParseStrn(req+2, config.host, 64);
		case 'o':
			if (sscanf(req+2, "%u", &(config.port))==0)
				return 0;
			return 1;
			break;
		case 'a':
			return configParseStrne(req+2, config.apiKey, 20);
		case 'c':
			return configParseStrne(req+2, config.clientID, 36);
		case 'x':
			if (sscanf(req+2, "%hhu", &(config.staticIP))==0)
				return 0;
			if (config.staticIP>1)
				return 0;
			return 1;
		default:
			return 0;
	}
	return 1;
}

void configInterrupt(){
	if (prevPinMode){
		interruptTriggerTime=millis();
		prevPinMode=0;
	} else { 
		prevPinMode=1;
		if (millis()-interruptTriggerTime>=2000)
			configEnabled=1;
	}
}