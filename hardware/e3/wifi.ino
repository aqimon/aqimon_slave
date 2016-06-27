#include <SoftwareSerial.h>
#include <string.h>
#define WIFI_TX 3
#define WIFI_RX 4
#define LINE_BUFFER_SIZE 64
#define WIFI_BUFFER_SIZE 256
#define LINE_DELIMITER "\r\n"

SoftwareSerial wifi(WIFI_RX, WIFI_TX);
char wifiBuffer[WIFI_BUFFER_SIZE], lineBuffer[LINE_BUFFER_SIZE];
int wifiMsgLen;
unsigned char lockNewLine=0, hasNewLine=0;

void wifiInit(){
	wifi.begin(19200);
  wifiExecute("ATE1");
  wifiExecute("AT+GMR");
  wifiExecute("AT+CWMODE_CUR=1");
}

void wifiExecute(char* command){
  wifi.write(command);
  wifi.write("\r\n");
  memset(wifiBuffer, 0, WIFI_BUFFER_SIZE);
  wifiMsgLen=0;
  while(1){
    wifiBufferLoop();
    if (lockNewLineBuffer()){
     Serial.println(lineBuffer);
     if (strstr(lineBuffer, "OK")!=NULL){
       Serial.println(F("end of command, ok received"));
       unlockNewLineBuffer();
       return;
     }
     if (strstr(lineBuffer, "ERROR")!=NULL){
       Serial.println(F("end of command, error received"));
       unlockNewLineBuffer();
       return;
     }
     unlockNewLineBuffer();
    }
  }
}

void wifiBufferLoop(){
  	while (wifi.available()){
      if (wifiMsgLen==WIFI_BUFFER_SIZE-2){
        Serial.println("overflow");
        return;
      }
     wifiBuffer[wifiMsgLen]=wifi.read();
     if (wifiBuffer[wifiMsgLen]=='\n' && (!lockNewLine)){
        wifiBuffer[wifiMsgLen]='\0';
        memmove(lineBuffer, wifiBuffer, wifiMsgLen+1);
        hasNewLine=1;
        wifiMsgLen=0;
     } else {
        wifiMsgLen++;
     }
  	}
}

unsigned char lockNewLineBuffer(){
  if (hasNewLine){
    hasNewLine=0;
    lockNewLine=1;
    return 1;
  } else {
    lockNewLine=0;
    return 0;
  }
}

void unlockNewLineBuffer(){
  lockNewLine=0;
}

void wifiLoop(){
  while(1){
  if (wifi.available())
    Serial.write(wifi.read());
  if (Serial.available())
    wifi.write(Serial.read());
  }
}

void wifiConnectToAP(char *ssid, char *password){
  char commandBuf[100];
  sprintf(commandBuf, "AT+CWJAP_CUR=\"%s\",\"%s\"", ssid, password);
  wifiExecute(commandBuf);
}

void wifiSendHTTPRequest(char *clientID, float temperature, float humidity, float dustLevel, float coLevel){
  wifiExecute("AT+CIPMUX=0");
  wifiExecute("AT+CIPSTART=\"TCP\",\"tuankiet69.mooo.com\",5000");
  char tempStr[6], humidStr[6], dustStr[5], coStr[5], sendBuffer[256], buffer2[30];
  int bufferLen;
  dtostrf(temperature, 5, 2, tempStr);
  dtostrf(humidity, 5, 2, humidStr);
  dtostrf(dustLevel, 4, 2, dustStr);
  dtostrf(coLevel, 4, 2, coStr);
  sprintf(sendBuffer, "GET /api/add/event?clientID=%s&temperature=%s&humidity=%s&dustLevel=%s&coLevel=%s HTTP/1.1\r\n"
                      "Host: tuankiet69.mooo.com\r\n"
                      "\r\n", clientID, tempStr, humidStr, dustStr, coStr);
  bufferLen=strlen(sendBuffer);
  sprintf(buffer2, "AT+CIPSEND=%d", bufferLen);
  wifiExecute(buffer2);
  wifiExecute(sendBuffer);
  wifiExecute("AT+CIPSTOP");
}

