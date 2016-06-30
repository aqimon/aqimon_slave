#include <avr/eeprom.h>

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