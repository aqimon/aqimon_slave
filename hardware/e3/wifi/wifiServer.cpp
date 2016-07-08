#include "wifiServer.h"

void wifiServerInit(){
	//wifiInit();
	wifiExecute("AT+CWMODE_CUR=3");
	wifiExecute("AT+CIPMUX=1");
	wifiExecute("AT+CIPSERVER=1,80");
}


void wifiServerListener(){
	int i, linkID, ptr;
	char httpMethod[8], httpPath[255], httpVer[9];
	while (1){
		wifiBufferLoop();
		if (hasNewLine()){
			if (strstr(useLineBuffer(), "+IPD") != NULL){
				// now we parse the request
				Serial.println(F("Get something, parsing to see if it's http or not"));
				sscanf_P(useLineBuffer(),
						 PSTR("+IPD,%d,%*d:%7s %255s %s\r\n"),
						 &linkID,
						 httpMethod, // max length 7 chars
						 httpPath, // max length 255 chars
						 httpVer); // max length 9 chars 
				Serial.println(useLineBuffer());
				Serial.println(linkID);
				Serial.println(httpMethod);
				Serial.println(httpPath);
				Serial.println(httpVer);

				if ((!strcmp(httpVer, "HTTP/1.1")) || (!strcmp(httpVer, "HTTP/1.0"))){
					Serial.println("It's really a HTTP request");
					Serial.println("Gonna parse headers until see \\r\\n");
					while (strcmp(useLineBuffer(), "\r\n"))
						wifiBufferLoop();
					wifiSendIndex(0, 32);
					wifiSendIndex(32, 64);
					wifiExecute("AT+CIPCLOSE=0");
					//wifiSendIndex(256, 511);
					Serial.println("Finish sending");
				}
			}
		}
	}
}

void wifiSendIndex(int from, int to){
	static char buffer[32], *ptr2=buffer;
	int ptr;
	for (ptr=from; ptr<to; ptr++){
		*ptr2=pgm_read_byte(index+ptr);
		ptr2++;
	}
	Serial.println(buffer);
	Serial.flush();
	wifi.write("AT+CIPSENDEX=");
	wifi.print(0);
	wifi.write(",");
	wifi.print(to-from+1);
	wifi.write("\r\n");
	delay(1000);
	wifi.write(buffer);
	wifi.write("\r\n");
	delay(2000);
}

void wifiServerWriteIndexToSerial(){
	int i=0;
	char chr;
	while (chr=pgm_read_byte(index+i)){
		Serial.write(chr);
		i++;
	}
}