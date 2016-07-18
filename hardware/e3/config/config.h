#ifndef CONFIG_H

    #define CONFIG_H

    struct config_s {
        unsigned char flags;
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

    void configRead();
    void configWrite();
    void fillEEPROM();
    int configParseRequest(char *req)
#endif