#include "config.h"

struct config_s config;

void configRead() {
    eeprom_read_block((void*)&config, (void*)0, sizeof(config));
}

void configWrite() {
	Serial.println(config.clientID);
	Serial.println(config.apiKey);
    eeprom_update_block((void*)&config, (void*)0, sizeof(config));
}

void fillEEPROM() {
    int i;
    for (i = 0; i < 1024; i++) {
        eeprom_write_byte((uint8_t*)i, 0);
    }
}

int configParseIP(char *str, unsigned char *ip){
	int res;
	res=sscanf(str, "%u.%u.%u.%u", &(ip[0]), &(ip[1]), &(ip[2]), &(ip[3]));
	return (res==4);
}

int configParseStrn(char *src, char* dst, int n){
	if (strlen(src)>n)
		return 0;
	strcpy(dst, src);
	return 1;
}

int configParseStrne(char *src, char *dst, int n){
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
			break;
		case 's': // subnet
			return configParseIP(req+2, config.subnetMask);
			break;
		case 'g': // gateway
			return configParseIP(req+2, config.gateway);
			break;
		case 'd': // ssid name
			configParseStrn(req+2, config.ssidName, 31);
			break;
		case 'p': // password
			configParseStrn(req+2, config.ssidPassword, 63);
			break;
		case 'h':
			configParseStrn(req+2, config.host, 64);
			break;
		case 'o':
			if (sscanf(req+2, "%u", &(config.port))==0)
				return 0;
			break;
		case 'a':
			configParseStrne(req+2, config.apiKey, 20);
			break;
		case 'c':
			configParseStrne(req+2, config.clientID, 36);
			break;
		case 'x':
			unsigned char tmp;
			if (sscanf(req+2, "%u", &tmp)==0)
				return 0;
			if (tmp>1)
				return 0;
			config.staticIP=tmp;
			break;
	}
	return 1;
}