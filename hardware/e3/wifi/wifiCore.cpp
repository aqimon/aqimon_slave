#include "wifiCore.h"

SoftwareSerial wifi(WIFI_RX, WIFI_TX);
char wifiBuffer[WIFI_BUFFER_SIZE], lineBuffer[LINE_BUFFER_SIZE];
int wifiMsgLen;
unsigned char newLine = 0;

void wifiInit() {
    fdev_setup_stream(&stream, wifiStreamSend, NULL, _FDEV_SETUP_WRITE);
    lcdUpdateWifiStatus(WIFI_STARTING);
    pinMode(WIFI_RST, OUTPUT);
    digitalWrite(WIFI_RST, LOW);
    delay(1000);
    digitalWrite(WIFI_RST, HIGH);
    delay(1000);
    wifi.begin(9600); // the lowest baudrate that works, 1200 don't work. 2400 is unstable, 4800?
    wifiExecute(PSTR("ATE1"));
    wifiExecute(PSTR("AT+GMR"));
    wifiExecute(PSTR("AT+CWMODE_CUR=3"));
}

unsigned char wifiExecute(char *command) {
    wifi.write(command);
    wifi.write("\r\n");
    return wifiWaitForResult();
}

unsigned char wifiExecute(PGM_P command){
    char c;
    while ((c=pgm_read_byte(command))!='\0'){
        wifi.write(c);
        command++;
    }
    wifi.write("\r\n");
    return wifiWaitForResult();
}

unsigned char wifiWaitForResult(){
    memset(wifiBuffer, 0, WIFI_BUFFER_SIZE);
    wifiMsgLen = 0;
    unsigned long t=millis();
    while (1) {
        wifiBufferLoop();
        if (hasNewLine()) {
            if (strcmp(useLineBuffer(), "OK\r\n")==0
                || strcmp(useLineBuffer(), "SEND OK\r\n")==0) {
                Serial.println(F("end of command, ok received"));
                return 1;
            }
            if (strstr(useLineBuffer(), "ERROR")!=NULL) {
                Serial.println(F("end of command, error received"));
                lcdUpdateWifiStatus(WIFI_RESET);
                return 0;
            }
        }
        if (millis()-t>=TIMEOUT){
            Serial.println(F("Timeout"));
            lcdUpdateWifiStatus(WIFI_TIMEOUT);
            return 0;
        }
    }
}

void wifiBufferLoop() {
    if (wifi.available()) {
        if (wifiMsgLen == WIFI_BUFFER_SIZE - 2) {
            Serial.println(F("overflow, erasing buffer"));
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

int wifiSendLength=0, wifiSendLinkID=0;
FILE stream;

unsigned char wifiInitiateSend(char linkID){
    wifi.write("AT+CIPSENDEX=");
    if (linkID!=-1)
        wifiSendLinkID=linkID;
    wifi.print(wifiSendLinkID);
    wifi.write(",");
    wifi.write("2048\r\n");
    wifiSendLength=0;
    do
        wifiBufferLoop();
    while (strcmp(useLineBuffer(), ">")!=0);
    return 0;
}

int wifiStreamSend(char c, FILE *stream){
    if (wifiSendLength%2048==0 && wifiSendLength!=0)
        wifiInitiateSend(-1);
    wifi.write(c);
    wifiSendLength++;
    if (wifiSendLength%2048==0)
        wifiWaitForResult();
    return 0;
}

unsigned char wifiEndSend(){
    if (wifiSendLength%2048!=0){
        wifi.write("\\0");
        wifiWaitForResult();
    }
    wifi.write("AT+CIPCLOSE=");
    wifi.print(wifiSendLinkID);
    wifi.write("\r\n");
    return wifiWaitForResult();
}