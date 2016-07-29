#include "wifiCore.h"

AltSoftSerial wifi(WIFI_RX, WIFI_TX);
char wifiBuffer[WIFI_BUFFER_SIZE], lineBuffer[LINE_BUFFER_SIZE];
int wifiMsgLen, wifiSendLength = 0, wifiSendLinkID = 0;
unsigned char newLine = 0;
FILE stream, commandStream;

void wifiInit() {
    fdev_setup_stream(&stream, wifiStreamSend, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&commandStream, wifiStreamSendCommand, NULL, _FDEV_SETUP_WRITE);
    lcdUpdateWifiStatus(WIFI_STARTING);
    wifi.begin(38400); // Thanks AltSoftSerial!
    while (!wifiReset())
        Serial.println(F("Cannot hard reset, retrying"));
    wifiExecute(PSTR("AT"));
    wifiExecute(PSTR("AT+GMR"));
    wifiExecute(PSTR("AT+RFVDD"));
}

unsigned char wifiReset() {
    pinMode(WIFI_RST, OUTPUT);
    digitalWrite(WIFI_RST, LOW);
    delay(500);
    digitalWrite(WIFI_RST, HIGH);
    unsigned long t=millis();
    while (millis()-t<=3000){
        wifiBufferLoop();
        if (hasNewLine() && strcmp_P(useLineBuffer(), PSTR("ready\r"))==0)
            return 1;
    }
    delay(1000);
    return 0;
}

unsigned char wifiSetStaticIP() {
    fprintf_P(&commandStream, PSTR("AT+CIPSTA_CUR=\"%d.%d.%d.%d\",\"%d.%d.%d.%d\",\"%d.%d.%d.%d\"\r\n"),
              config.ip[0], config.ip[1], config.ip[2], config.ip[3],
              config.gateway[0], config.gateway[1], config.gateway[2], config.gateway[3],
              config.subnetMask[0], config.subnetMask[1], config.subnetMask[2], config.subnetMask[3]);
    return wifiWaitForResult();
}

unsigned char wifiExecute(PGM_P command) {
    fputs_P(command, &commandStream);
    fputs("\r\n", &commandStream);
    return wifiWaitForResult();
}

unsigned char wifiWaitForResult() {
    memset(wifiBuffer, 0, WIFI_BUFFER_SIZE);
    wifiMsgLen = 0;
    unsigned long t = millis();
    while (1) {
        if ((configEnabled == 1) && (configStarted == 0)) {
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
                    || strcmp(useLineBuffer(), "FAIL\r") == 0) {
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
    wdt_reset();
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
            newLine = 1;
            wifiMsgLen = 0;
        } else if (wifiBuffer[wifiMsgLen] == '>') {
            wifiBuffer[wifiMsgLen + 1] = '\0';
            memmove(lineBuffer, wifiBuffer, wifiMsgLen + 2);
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
        wdt_reset();
        if (wifi.available())
            Serial.write(wifi.read());
        if (Serial.available())
            wifi.write(Serial.read());
    }
}

unsigned char wifiDeepSleep(unsigned long duration){
    fprintf_P(&commandStream, PSTR("AT+GSLP=%lu\r\n"), duration);
    return wifiWaitForResult();
}

unsigned char wifiInitiateSend(int linkID) {
    wifi.write("AT+CIPSENDEX=");
    if (linkID != -1)
        wifiSendLinkID = linkID;
    wifi.print(wifiSendLinkID);
    wifi.write(",256\r\n");
    wifiSendLength = 0;
    do
        wifiBufferLoop();
    while (strcmp(useLineBuffer(), ">") != 0);
    return 0;
}

int wifiStreamSend(char c, FILE *stream) {
    if (wifiSendLength % 256 == 0 && wifiSendLength != 0)
        wifiInitiateSend(-1);
    wifi.write(c);
    wifiSendLength++;
    if (wifiSendLength % 256 == 0)
        wifiWaitForResult();
    return 0;
}

int wifiStreamSendCommand(char c, FILE *stream) {
    wifi.write(c);
    return 0;
}

unsigned char wifiEndSend() {
    if (wifiSendLength % 256 != 0) {
        wifi.write("\\0");
        wifiWaitForResult();
    }
    wifi.write("AT+CIPCLOSE=");
    wifi.print(wifiSendLinkID);
    wifi.write("\r\n");
    return wifiWaitForResult();
}