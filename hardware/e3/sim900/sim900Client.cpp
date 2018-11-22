#include "sim900Client.h"

void sim900ClientInit() {
    lcdUpdateWifiStatus(3);
    sim900Execute(PSTR("AT+CSTT=\"m-wap\",\"mms\",\"mms\""));
    delay(2000);
    sim900Execute(PSTR("AT+CIICR"));
    fprintf_P(&commandStream, PSTR("AT+CIFSR\r\n"));
    delay(1000);
}

unsigned char sim900SendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel) {
    char tempStr[6],
         humidStr[6],
         dustStr[5],
         coStr[5];

    dtostrf(temperature, 5, 2, tempStr);
    dtostrf(humidity, 5, 2, humidStr);
    dtostrf(dustLevel, 4, 2, dustStr);
    dtostrf(coLevel, 4, 2, coStr);

    if (humidStr[0] == ' ')
        humidStr[0] = '0';
    
    lcdUpdateWifiStatus(4);
    fprintf_P(&commandStream, PSTR("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n"), config.host, config.port);
    unsigned long t = millis();
    do {
        sim900BufferLoop();
        if (strcmp_P(useLineBuffer(), PSTR("ERROR\r")) == 0) return 0;
        if ((millis() - t) > 30000) return 0;
    } while (strcmp_P(useLineBuffer(), PSTR("CONNECT OK\r")) != 0);

    sim900InitiateSend();
    lcdUpdateWifiStatus(5);
    fprintf_P(&stream, PSTR("GET /api/add/event?client_id=%s&temperature=%s&humidity=%s&dustlevel=%s&colevel=%s&apikey=%s HTTP/1.1\r\n"
                            "Host: %s\r\n"
                            "\r\n"),
              config.clientID,
              tempStr,
              humidStr,
              dustStr,
              coStr,
              config.apiKey,
              config.host);

    sim900EndSend();
    lcdUpdateWifiStatus(6);
    return 1;
}
