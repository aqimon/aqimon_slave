#include <avr/eeprom.h>
#include "config.h"

struct config_s config;

void configRead() {
    eeprom_read_block((void*)&config, (void*)0, sizeof(config));
}

void configWrite() {
    eeprom_update_block((void*)&config, (void*)0, sizeof(config));
}

void fillEEPROM() {
    int i;
    for (i = 0; i < 1024; i++) {
        eeprom_write_byte((uint8_t*)i, 0);
    }
}

int configParseRequest(char *req){
	if (req[1]!='='){
		configRead();
		return 0;
	}
	switch (req[0]){
		case 'i': // ip
			break;
		case 's': // subnet
			break;
		case 'g': // gateway
			break;
		case 'd': // ssid name
			if (strlen(req+2)>31)
				return 0;
			strcpy(req+2, config.ssidName);
			break;
		case 'p': // password
			if (strlen(req+2)>63)
				return 0;
			strcpy(req+2, config.ssidPassword);
			break;
		case 'h':
			if (strlen(req+2)>64)
				return 0;
			strcpy(req+2, config.host);
			break;
		case 'o':
			if (sscanf(req+2, "%u", &(config.port)==0))
				return 0;
			break;
		case 'a':
			if (strlen(req+2)!=20)
				return 0;
			strcpy(req+2, config.apiKey);
			break;
		case 'c':
			if (strlen(req+2)!=36)
				return 0;
			strcpy(req+2, config.clientID);
			break;
		default:
			configRead();
			return 0;
	}
	return 1;
}