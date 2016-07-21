#include "wifiClient.h"

void wifiClientInit() {
    if (config.staticIP)
        wifiSetStaticIP();
    wifiExecute(PSTR("AT+CWMODE_CUR=1"));
    wifiExecute(PSTR("AT+CIPMUX=1"));
    wifiConnectToAP();
}

unsigned char wifiConnectToAP() {
    lcdUpdateWifiStatus(WIFI_CONNECTING_AP);
    fprintf_P(&stream, PSTR("AT+CWJAP_CUR=\"%s\",\"%s\"\r\n"), config.ssidName, config.ssidPassword);
    return wifiWaitForResult();
}

unsigned char wifiSendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel) {
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
    
    lcdUpdateWifiStatus(WIFI_CONNECTING_HTTP);
    fprintf_P(&stream, PSTR("AT+CIPSTART=0,\"TCP\",\"%s\",%d\r\n"), config.host, config.port);
    if (!wifiWaitForResult())
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
    return 1;
}