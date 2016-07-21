#include "wifiCore.h"

AltSoftSerial wifi(WIFI_RX, WIFI_TX);
char wifiBuffer[WIFI_BUFFER_SIZE], lineBuffer[LINE_BUFFER_SIZE];
int wifiMsgLen, wifiSendLength = 0, wifiSendLinkID = 0;
unsigned char newLine = 0;
FILE stream;

void wifiInit() {
    fdev_setup_stream(&stream, wifiStreamSend, NULL, _FDEV_SETUP_WRITE);
    lcdUpdateWifiStatus(WIFI_STARTING);
    pinMode(WIFI_RST, OUTPUT);
    digitalWrite(WIFI_RST, LOW);
    delay(1000);
    digitalWrite(WIFI_RST, HIGH);
    delay(1000);
    wifi.begin(38400); // Thanks AltSoftSerial!
    wifiExecute(PSTR("ATE1"));
    wifiExecute(PSTR("AT+GMR"));
    wifiExecute(PSTR("AT+CWDHCP_DEF=2,1"));
}

unsigned char wifiSetStaticIP() {
    fprintf_P(&stream, PSTR("AT+CIPSTA_CUR=\"%d.%d.%d.%d\",\"%d.%d.%d.%d\",\"%d.%d.%d.%d\"\r\n"),
              config.ip[0], config.ip[1], config.ip[2], config.ip[3],
              config.gateway[0], config.gateway[1], config.gateway[2], config.gateway[3],
              config.subnetMask[0], config.subnetMask[1], config.subnetMask[2], config.subnetMask[3]);
    return wifiWaitForResult();
}

unsigned char wifiExecute(PGM_P command) {
    fputs_P(command, &stream);
    fputs("\r\n", &stream);
    return wifiWaitForResult();
}

unsigned char wifiWaitForResult() {
    memset(wifiBuffer, 0, WIFI_BUFFER_SIZE);
    wifiMsgLen = 0;
    unsigned long t = millis();
    while (1) {
        if ((configEnabled==1) && (configStarted==0)) {
            configStarted = 1;
            wifiInit();
            wifiServerInit();
            wifiServerListener();
        }
        wifiBufferLoop();
        if (hasNewLine()) {
            if (strcmp(useLineBuffer(), "OK\r") == 0
                    || strcmp(useLineBuffer(), "SEND OK\r") == 0) {
                Serial.println(F("end of command, ok received"));
                return 1;
            }
            if (strcmp(useLineBuffer(), "ERROR\r") == 0
                    || strcmp(useLineBuffer(), "FAIL\r") == 011) {
                Serial.println(F("end of command, error received"));
                lcdUpdateWifiStatus(WIFI_RESET);
                return 0;
            }
        }
        if (millis() - t >= TIMEOUT) {
            Serial.println(F("Timeout"));
            lcdUpdateWifiStatus(WIFI_TIMEOUT);
            return 0;
        }
    }
}

void wifiBufferLoop() {
    if (wifi.available()) {
        if (wifiMsgLen == WIFI_BUFFER_SIZE - 1) {
            Serial.println(F("overflow, erasing buffer"));
            wifiMsgLen = 0;
            return;
        }
        wifiBuffer[wifiMsgLen] = wifi.read();
        Serial.write(wifiBuffer[wifiMsgLen]);
        if (wifiBuffer[wifiMsgLen] == '\n') {
            wifiBuffer[wifiMsgLen] = '\0';
            memmove(lineBuffer, wifiBuffer, wifiMsgLen + 1);
            //Serial.println(lineBuffer);
            newLine = 1;
            wifiMsgLen = 0;
        } else if (wifiBuffer[wifiMsgLen] == '>') {
            wifiBuffer[wifiMsgLen + 1] = '\0';
            memmove(lineBuffer, wifiBuffer, wifiMsgLen + 2);
            //Serial.println(lineBuffer);
            newLine = 1;
            wifiMsgLen = 0;
        } else {
            wifiMsgLen++;
        }
    }
}

char* useLineBuffer() {
    newLine = 0;
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

unsigned char wifiInitiateSend(int linkID) {
    wifi.write("AT+CIPSENDEX=");
    if (linkID != -1)
        wifiSendLinkID = linkID;
    wifi.print(wifiSendLinkID);
    wifi.write(",");
    wifi.write("512\r\n");
    wifiSendLength = 0;
    do
        wifiBufferLoop();
    while (strcmp(useLineBuffer(), ">") != 0);
    return 0;
}

int wifiStreamSend(char c, FILE *stream) {
    if (wifiSendLength % 512 == 0 && wifiSendLength != 0)
        wifiInitiateSend(-1);
    wifi.write(c);
    wifiSendLength++;
    if (wifiSendLength % 512 == 0)
        wifiWaitForResult();
    return 0;
}

unsigned char wifiEndSend() {
    if (wifiSendLength % 512 != 0) {
        wifi.write("\\0");
        wifiWaitForResult();
    }
    wifi.write("AT+CIPCLOSE=");
    wifi.print(wifiSendLinkID);
    wifi.write("\r\n");
    return wifiWaitForResult();
}