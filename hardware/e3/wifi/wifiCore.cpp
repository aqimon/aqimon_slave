#include "wifiCore.h"

SoftwareSerial wifi(WIFI_RX, WIFI_TX);
char wifiBuffer[WIFI_BUFFER_SIZE], lineBuffer[LINE_BUFFER_SIZE];
int wifiMsgLen;
unsigned char newLine = 0;

void wifiInit() {
    lcdUpdateWifiStatus(WIFI_STARTING);
    pinMode(WIFI_RST, OUTPUT);
    digitalWrite(WIFI_RST, LOW);
    delay(1000);
    digitalWrite(WIFI_RST, HIGH);
    delay(1000);
    wifi.begin(2400); // the lowest baudrate that works, 1200 don't work. 2400 is unstable, 4800?
    wifiExecute("ATE1");
    wifiExecute("AT+GMR");
    wifiExecute("AT+CWMODE_CUR=1");
    wifiExecute("AT+CIPSTA_CUR=\"192.168.0.169\",\"192.168.0.1\",\"255.255.255.0\"");
    while (!wifiConnectToAP());
}

unsigned char wifiExecute(char *command) {
    wifi.write(command);
    wifi.write("\r\n");
    memset(wifiBuffer, 0, WIFI_BUFFER_SIZE);
    wifiMsgLen = 0;
    unsigned long t=millis();
    while (1) {
        wifiBufferLoop();
        if (hasNewLine()) {
            if (strstr(useLineBuffer(), "OK") != NULL) {
                Serial.println(F("end of command, ok received"));
                return 1;
            }
            if (strstr(useLineBuffer(), "ERROR") != NULL) {
                Serial.println(F("end of command, error received"));
                lcdUpdateWifiStatus(WIFI_RESET);
                return 0;
            }
        }
        if (millis()-t>=TIMEOUT){
            Serial.println(F("Timeout"));
            lcdUpdateWifiStatus(WIFI_TIMEOUT);
            wifiInit();
            return 0;
        }
    }
}

void wifiBufferLoop() {
    if (wifi.available()) {
        if (wifiMsgLen == WIFI_BUFFER_SIZE - 2) {
            Serial.println("overflow, erasing buffer");
            wifiMsgLen=0;
            return;
        }
        wifiBuffer[wifiMsgLen] = wifi.read();
        if (wifiBuffer[wifiMsgLen] == '\n' || wifiBuffer[wifiMsgLen] == '>') {
            wifiBuffer[wifiMsgLen+1] = '\0';
            memmove(lineBuffer, wifiBuffer, wifiMsgLen + 2);
            Serial.print(lineBuffer);
            newLine = 1;
            wifiMsgLen = 0;
        } else {
            wifiMsgLen++;
        }
    }
}

char* useLineBuffer(){
    newLine=0;
    return lineBuffer;
}
unsigned char hasNewLine() {
    return newLine;
}

void wifiLoop() {
    while (1) {
        if (wifi.available())
            Serial.write(wifi.read());
        if (Serial.available())
            wifi.write(Serial.read());
    }
}

unsigned char wifiConnectToAP() {
    lcdUpdateWifiStatus(WIFI_CONNECTING_AP);
    char commandBuf[128];
    sprintf_P(commandBuf, PSTR("AT+CWJAP_CUR=\"%s\",\"%s\""), config.ssidName, config.ssidPassword);
    return wifiExecute(commandBuf);
}