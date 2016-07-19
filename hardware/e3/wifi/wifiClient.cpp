#include "wifiClient.h"

void wifiClientInit(){
    if (config.staticIP)
        wifiSetStaticIP();
    wifiExecute(PSTR("AT+CWMODE_CUR=1"));
    wifiExecute(PSTR("AT+CIPMUX=1"));
    wifiConnectToAP();
}

unsigned char wifiConnectToAP() {
    lcdUpdateWifiStatus(WIFI_CONNECTING_AP);
    char commandBuf[128];
    sprintf_P(commandBuf, PSTR("AT+CWJAP_CUR=\"%s\",\"%s\""), config.ssidName, config.ssidPassword);
    return wifiExecute(commandBuf);
}

unsigned char wifiSendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel) {
    char tempStr[6],
         humidStr[6],
         dustStr[5],
         coStr[5],
         sendBuffer[200],
         tmp[16];

    dtostrf(temperature, 5, 2, tempStr);
    dtostrf(65.0, 5, 2, humidStr);
    dtostrf(dustLevel, 4, 2, dustStr);
    dtostrf(coLevel, 4, 2, coStr);

    lcdUpdateWifiStatus(WIFI_CONNECTING_HTTP);
    sprintf_P(sendBuffer, PSTR("AT+CIPSTART=0,\"TCP\",\"%s\",%d"), config.host, config.port);
    if (!wifiExecute(sendBuffer))
        return 0;

    lcdUpdateWifiStatus(WIFI_SENDING);

    wifiInitiateSend(0);
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

    wifiEndSend();
    lcdUpdateWifiStatus(WIFI_SENDOK);
}