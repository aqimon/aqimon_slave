#include "wifiClient.h"

void wifiClientInit() {
    if (config.staticIP)
        wifiSetStaticIP();
    wifiExecute(PSTR("AT+CWMODE_CUR=1"));
    wifiExecute(PSTR("AT+CWDHCP_DEF=0,1"));
    wifiExecute(PSTR("AT+CIPMUX=1"));
    wifiConnectToAP();
}

unsigned char wifiConnectToAP() {
    lcdUpdateWifiStatus(WIFI_CONNECTING_AP);
    fprintf_P(&commandStream, PSTR("AT+CWJAP_CUR=\"%s\",\"%s\"\r\n"), config.ssidName, config.ssidPassword);
    return wifiWaitForResult();
}

unsigned char wifiSendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel) {
    lcdUpdateWifiStatus(WIFI_CONNECTING_HTTP);
    fprintf_P(&commandStream, PSTR("AT+CIPSTART=0,\"TCP\",\"%s\",%d\r\n"), config.host, config.port);
    if (!wifiWaitForResult())
        return 0;

    lcdUpdateWifiStatus(WIFI_SENDING);

    wifiInitiateSend(0);
    fprintf_P(&stream, PSTR("GET /api/add/event?client_id=%s&temperature=%f&humidity=%f&dustlevel=%f&colevel=%f&apikey=%s HTTP/1.1\r\n"
                            "Host: %s\r\n"
                            "\r\n"),
              config.clientID,
              temperature,
              humidity,
              dustLevel,
              coLevel,
              config.apiKey,
              config.host);

    wifiEndSend();
    lcdUpdateWifiStatus(WIFI_SENDOK);
    return 1;
}