#include "wifiClient.h"

unsigned char wifiSendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel) {
    char tempStr[6],
         humidStr[6],
         dustStr[5],
         coStr[5],
         sendBuffer[200],
         tmp[16];

    dtostrf(temperature, 5, 2, tempStr);
    dtostrf(humidity, 5, 2, humidStr);
    dtostrf(dustLevel, 4, 2, dustStr);
    dtostrf(coLevel, 4, 2, coStr);

    if (!wifiExecute("AT+CIPMUX=0"))
        return 0;
    lcdUpdateWifiStatus(WIFI_CONNECTING_HTTP);
    sprintf_P(sendBuffer, PSTR("AT+CIPSTART=\"TCP\",\"%s\",%d"), config.host, config.port);
    if (!wifiExecute(sendBuffer))
        return 0;

    lcdUpdateWifiStatus(WIFI_SENDING);
    int bufferLen;
    sprintf_P(sendBuffer, PSTR("GET /api/add/event?client_id=%s&temperature=%s&humidity=%s&dustlevel=%s&colevel=%s&apikey=%s HTTP/1.1\r\n"
                               "Host: %s\r\n"
                               "\r\n"),
              config.clientID,
              tempStr,
              humidStr,
              dustStr,
              coStr,
              config.apiKey,
              config.host);

    bufferLen = strlen(sendBuffer);
    sprintf_P(tmp, PSTR("AT+CIPSEND=%d"), bufferLen);
    if (!wifiExecute(tmp))
        return 0;
    if (!wifiExecute(sendBuffer))
        return 0;
    if (!wifiExecute("AT+CIPCLOSE"))
        return 0;
    lcdUpdateWifiStatus(WIFI_SENDOK);
}