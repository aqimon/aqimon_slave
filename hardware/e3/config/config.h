#ifndef CONFIG_H

    #define CONFIG_H
    
    #include <avr/eeprom.h>
    #include <stdio.h>
    #include <string.h>
    #include <Arduino.h>

    struct config_s {
        unsigned char staticIP;
        unsigned char ip[4];
        unsigned char subnetMask[4];
        unsigned char gateway[4];
        char ssidName[32];
        char ssidPassword[64];
        char host[65];
        uint16_t port;
        char apiKey[21];
        char clientID[37];
    };

    extern struct config_s config;
    extern volatile unsigned long interruptTriggerTime;
    extern volatile unsigned char configEnabled, configStarted;

    void configRead();
    void configWrite();
    void fillEEPROM();
    int configParseRequest(char *req);
    void configInterrupt();

#endif