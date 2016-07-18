#include "wifiServer.h"

void wifiServerInit() {
	wifiExecute(PSTR("AT+CWMODE_CUR=3"));
	wifiExecute(PSTR("AT+CWSAP_CUR=\"MakerWeek\",\"\",5,0,4,0"));
	wifiExecute(PSTR("AT+CWDHCP_CUR=0,1"));
	wifiExecute(PSTR("AT+CWDHCPS_CUR=1,60,\"192.168.69.69\",\"192.168.69.96\""));
	wifiExecute(PSTR("AT+CIPAP_CUR=\"192.168.69.1\",\"192.168.69.0\",\"255.255.255.0\""));
	wifiExecute(PSTR("AT+CIPMUX=1"));
	wifiExecute(PSTR("AT+CIPSERVER=1,80"));
}


void wifiServerListener() {
	char httpMethod[8], httpPath[16], httpVer[9];
	unsigned char linkID;
	int res;
	while (1) {
		wifiBufferLoop();
		if (hasNewLine()) {
			if (strstr(useLineBuffer(), "+IPD") != NULL) {
				// now we parse the request
				Serial.println(F("Get something, parsing to see if it's http or not"));
				res = sscanf_P(useLineBuffer(),
				               PSTR("+IPD,%d,%*d:%7s %16s %s\r\n"),
				               &linkID,
				               httpMethod, // max length 7 chars
				               httpPath, // max length 16 chars
				               httpVer); // max length 9 chars
				if (res!=4){
					wifiInitiateSend(linkID);
					wifiServerSend400();
					wifiEndSend();
					continue;
				}
				if ((!strcmp_P(httpVer, PSTR("HTTP/1.1"))) || (!strcmp_P(httpVer, PSTR("HTTP/1.0")))) {
					Serial.println(F("It's really a HTTP request"));
					while (strcmp(useLineBuffer(), "\r\n") != 0)
						wifiBufferLoop();
					if (strcmp(httpPath, "/") == 0) {
						Serial.println(F("Sending index page"));
						wifiInitiateSend(linkID);
						wifiServerSend200(&stream);
						wifiServerSendIndex(&stream);
						wifiEndSend();
					} else if (strcmp(httpPath, "/ssidlist") == 0) {
						Serial.println(F("Sending SSID list"));
						wifiServerSendSSID(&stream, linkID);
						wifiEndSend();
					} else if (strcmp(httpPath, "/restart") == 0) {
						Serial.println(F("Restarting"));
						wifiInitiateSend(linkID);
						wifiServerSend200JSON();
						wifiServerSendSuccess();
						wifiEndSend();
						// TODO: restart
					} else if (strcmp(httpPath, "/") == 0 && strcmp(httpMethod, "POST") == 0) {
						Serial.println(F("Saving settings"));
						do
							wifiBufferLoop();
							if (hasNewLine()){
								if (!configPassRequest(useLineBuffer())){
									wifiInitiateSend(linkID);
									wifiServerSend400();
									wifiEndSend();
									break;
								}
							}
						while (strcmp(useLineBuffer(), "end=yes")!=0);
						wifiInitiateSend(linkID);
						wifiServerSend200JSON();
						wifiServerSendSuccess();
						wifiEndSend();
					} else {
						Serial.println(F("Sending 404"));
						wifiInitiateSend(linkID);
						wifiServerSend404(&stream);
						wifiEndSend();
					}
				} else {
					wifiInitiateSend(linkID);
					wifiServerSend400();
					wifiEndSend();
				}
			}
		}
	}
}

void wifiServerSendIndex(FILE *stream) {
	fprintf_P(stream, index);
}

int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void wifiServerSendSSID(FILE *stream, char linkID) {
	int ssidCount = 0, i = 0;
	struct ssid ssidList[5];
	wifi.write("AT+CWLAP\r\n");
	while (strcmp(useLineBuffer(), "OK\r\n") != 0) {
		wifiBufferLoop();
		if (hasNewLine() && ssidCount < 5) {
			if (strstr(useLineBuffer(), "+CWLAP:") != NULL) {
				sscanf_P(useLineBuffer(),
				         PSTR("+CWLAP:(%d,\"%31[0-9a-zA-Z ]\",%d,\"%17[0-9a-f:]\",%d,%*d,%*d)\r\n"),
				         &(ssidList[ssidCount].encryption),
				         ssidList[ssidCount].ssid,
				         &(ssidList[ssidCount].rssi),
				         ssidList[ssidCount].mac,
				         &(ssidList[ssidCount].channel));
				ssidCount++;
			}
		}
	}
	wifiInitiateSend(linkID);
	wifiServerSend200JSON(stream);
	fprintf(stream, "[");
	for (i = 0; i < ssidCount; i++) {
		fprintf_P(stream,
		          PSTR("{\"e\":%d,\"s\":\"%s\",\"r\":%d,\"m\":\"%s\",\"c\":%d}"),
		          ssidList[i].encryption,
		          ssidList[i].ssid,
		          ssidList[i].rssi,
		          ssidList[i].mac,
		          ssidList[i].channel);
		if (i != (ssidCount - 1))
			fprintf(stream, ",");
	}
	fprintf(stream, "]");
}

void wifiServerSend404(FILE* stream) {
	fprintf_P(stream, http404);
}

void wifiServerSend400(FILE* stream) {
	fprintf_P(stream, http400);
}

void wifiServerSend200(FILE* stream) {
	fprintf_P(stream, http200);
}

void wifiServerSend200JSON(FILE* stream) {
	fprintf_P(stream, http200json);
}

void wifiServerSendSuccess(FILE* stream) {
	fprintf_P(stream, success);
}