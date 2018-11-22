#include "sim900Core.h"

AltSoftSerial sim900(SIM900_RX, SIM900_TX);
char sim900Buffer[SIM900_BUFFER_SIZE], lineBuffer[LINE_BUFFER_SIZE];
int sim900SendLinkID = 0, sim900MsgLen;
unsigned char newLine = 0;
FILE stream, commandStream;

void sim900Init() {
    fdev_setup_stream(&stream, sim900StreamSend, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&commandStream, sim900StreamSendCommand, NULL, _FDEV_SETUP_WRITE);
    sim900.begin(38400); // Thanks AltSoftSerial!
    //sim900Loop();
    while (!sim900Reset())
        Serial.println(PSTR("Cannot hard reset, retrying"));
}

unsigned char sim900Reset() {
    lcdUpdateWifiStatus(1);
    pinMode(SIM900_RST, OUTPUT);
    digitalWrite(SIM900_RST, LOW);
    delay(500);
    digitalWrite(SIM900_RST, HIGH);
    unsigned long t=millis();
    lcdUpdateWifiStatus(2);
    while (millis()-t<=30000){
        sim900BufferLoop();
        if (hasNewLine() && strcmp_P(useLineBuffer(), PSTR("Call Ready\r")) == 0)
            return 1;
    }
    return 0;
}

unsigned char sim900Execute(PGM_P command) {
    fputs_P(command, &commandStream);
    fputs("\r\n", &commandStream);
    return sim900WaitForResult();
}

unsigned char sim900WaitForResult() {
    memset(sim900Buffer, 0, SIM900_BUFFER_SIZE);
    sim900MsgLen = 0;
    unsigned long t = millis();
    while (1) {
        sim900BufferLoop();
        if (hasNewLine()) {
            if (strcmp_P(useLineBuffer(), PSTR("OK\r")) == 0
                    || strcmp_P(useLineBuffer(), PSTR("SEND OK\r")) == 0
                    || strcmp_P(useLineBuffer(), PSTR("SHUT OK\r")) == 0
                    || strcmp_P(useLineBuffer(), PSTR("CLOSE OK\r")) == 0) {
                Serial.println(F("end of command, ok received"));
                return 1;
            }
            if (strcmp_P(useLineBuffer(), PSTR("ERROR\r")) == 0
                    || strcmp_P(useLineBuffer(), PSTR("FAIL\r")) == 0
                    || strcmp_P(useLineBuffer(), PSTR("ready\r")) == 0) {
                Serial.println(F("end of command, error received"));
                return 0;
            }
        }
        if (millis() - t >= TIMEOUT) {
            Serial.println(F("Timeout"));
            return 0;
        }
    }
}

void sim900BufferLoop() {
    wdt_reset();
    if (sim900.available()) {
        if (sim900MsgLen == SIM900_BUFFER_SIZE - 1) {
            Serial.println(F("buffer overflow, move pointer to beginning"));
            sim900MsgLen = 0;
            return;
        }
        sim900Buffer[sim900MsgLen] = sim900.read();
        Serial.write(sim900Buffer[sim900MsgLen]);
        if (sim900Buffer[sim900MsgLen] == '\n') {
            sim900Buffer[sim900MsgLen] = '\0';
            memmove(lineBuffer, sim900Buffer, sim900MsgLen + 1);
            newLine = 1;
            sim900MsgLen = 0;
        } else if (sim900Buffer[sim900MsgLen] == '>') {
            sim900Buffer[sim900MsgLen + 1] = '\0';
            memmove(lineBuffer, sim900Buffer, sim900MsgLen + 2);
            newLine = 1;
            sim900MsgLen = 0;
        } else {
            sim900MsgLen++;
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

void sim900Loop() {
    while (1) {
        wdt_reset();
        if (sim900.available())
            Serial.write(sim900.read());
        if (Serial.available())
            sim900.write(Serial.read());
    }
}

unsigned char sim900InitiateSend() {
    fprintf_P(&commandStream, PSTR("AT+CIPSEND\r\n"));
    do
        sim900BufferLoop();
    while (strcmp(useLineBuffer(), ">") != 0);
    return 0;
}

int sim900StreamSend(char c, FILE *stream) {
    sim900.write(c);
    return 0;
}

int sim900StreamSendCommand(char c, FILE *stream) {
    sim900.write(c);
    return 0;
}

unsigned char sim900EndSend() {
    sim900.write('\x1a');
    sim900WaitForResult();
    fprintf_P(&commandStream, PSTR("AT+CIPCLOSE\r\n"));
    return sim900WaitForResult();
}