#include "wifiServer.h"

void wifiServerInit() {
	wifiExecute(PSTR("AT+CWMODE_CUR=2"));
	wifiExecute(PSTR("AT+CWSAP_CUR=\"MakerWeek\",\"\",5,0,2,0"));
	wifiExecute(PSTR("AT+CWDHCP_CUR=0,1"));
	wifiExecute(PSTR("AT+CIPAP_CUR=\"192.168.69.1\",\"192.168.69.0\",\"255.255.255.0\""));
	wifiExecute(PSTR("AT+CIPMUX=1"));
	wifiExecute(PSTR("AT+CIPSERVER=1,80"));
	lcdUpdateWifiStatus(WIFI_SERVER);
}

int freeRam () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void wifiServerListener() {
	char httpMethod[8], httpPath[16], httpVer[9];
	int linkID;
	int res;
	while (1) {
		wifiBufferLoop();
		if (hasNewLine()) {
			if (strstr(useLineBuffer(), "+IPD") != NULL) {
				// now we parse the request
				Serial.println(F("Get something, parsing to see if it's http or not"));
				res = sscanf_P(useLineBuffer(),
				               PSTR("+IPD,%d,%*d:%7s %15s %8s\r"),
				               &linkID,
				               httpMethod, // max length 7 chars
				               httpPath, // max length 16 chars
				               httpVer); // max length 9 chars
				Serial.println(linkID);
				if (res != 4) {
					wifiInitiateSend(linkID);
					wifiServerSend400(&stream);
					wifiEndSend();
					continue;
				}
				if ((!strcmp_P(httpVer, PSTR("HTTP/1.1"))) || (!strcmp_P(httpVer, PSTR("HTTP/1.0")))) {
					Serial.println(F("It's really a HTTP request"));
					while (strcmp(useLineBuffer(), "\r") != 0)
						wifiBufferLoop();
					if (strcmp(httpPath, "/") == 0) {
						Serial.println(F("Sending index page"));
						wifiInitiateSend(linkID);
						wifiServerSendIndex(&stream);
						wifiEndSend();
					} else if (strcmp_P(httpPath, PSTR("/ssidlist")) == 0) {
						Serial.println(F("Sending SSID list"));
						wifiServerSendSSID(&stream, linkID);
						wifiEndSend();
					} else if (strcmp_P(httpPath, PSTR("/restart")) == 0) {
						Serial.println(F("Restarting"));
						wifiInitiateSend(linkID);
						wifiServerSend200JSON(&stream);
						wifiServerSendSuccess(&stream);
						wifiEndSend();
						soft_restart();
					} else if (strcmp_P(httpPath, PSTR("/save")) == 0 && strcmp_P(httpMethod, PSTR("POST")) == 0) {
						Serial.println(F("Saving settings"));
						int fail = 0;
						while (strcmp(useLineBuffer(), "begin") != 0)
							wifiBufferLoop();
						while (1) {
							wifiBufferLoop();
							if (hasNewLine()) {
								if (strcmp(useLineBuffer(), "end") == 0)
									break;
								if (!configParseRequest(useLineBuffer())) {
									Serial.println("fail:");
									Serial.println(useLineBuffer());
									Serial.println("end");
									wifiInitiateSend(linkID);
									wifiServerSend400(&stream);
									wifiEndSend();
									fail = 1;
									break;
								}
							}
						}
						if (fail==0) {
							configWrite();
							Serial.println(linkID);
							wifiInitiateSend(linkID);
							wifiServerSend200JSON(&stream);
							wifiServerSendSuccess(&stream);
							wifiEndSend();
						}
					} else if (strcmp_P(httpPath, PSTR("/currSetting")) == 0) {
						Serial.println(F("Sending curr setting"));
						wifiInitiateSend(linkID);
						wifiServerSend200JSON(&stream);
						wifiServerSendConfig(&stream);
						wifiEndSend();
					} else {
						Serial.println(F("Sending 404"));
						wifiInitiateSend(linkID);
						wifiServerSend404(&stream);
						wifiEndSend();
					}
				} else {
					wifiInitiateSend(linkID);
					wifiServerSend400(&stream);
					wifiEndSend();
				}
			}
		}
	}
}

void wifiServerSendIndex(FILE *stream) {
	int i;
	fprintf_P(stream, http200gzip);
	for (i = 0; i < indexLen; i++)
		fputc(pgm_read_byte(index + i), stream);
}

void wifiServerSendConfig(FILE *stream) {
	configRead();
	fprintf_P(stream,
	          PSTR("{"
	               "\"ip\": \"%d.%d.%d.%d\","
	               "\"subnet\": \"%d.%d.%d.%d\","
	               "\"gateway\": \"%d.%d.%d.%d\","
	               "\"ssid\": \"%s\","
	               "\"password\": \"%s\","
	               "\"host\": \"%s\","
	               "\"port\": %d,"
	               "\"apiKey\": \"%s\","
	               "\"clientID\": \"%s\","
	               "\"static\": %d"
	               "}"),
	          config.ip[0], config.ip[1], config.ip[2], config.ip[3],
	          config.subnetMask[0], config.subnetMask[1], config.subnetMask[2], config.subnetMask[3],
	          config.gateway[0], config.gateway[1], config.gateway[2], config.gateway[3],
	          config.ssidName,
	          config.ssidPassword,
	          config.host,
	          config.port,
	          config.apiKey,
	          config.clientID,
	          config.staticIP);
}

void wifiServerSendSSID(FILE *stream, char linkID) {
	int ssidCount = 0, i = 0;
	struct ssid ssidList[5];
	wifi.write("AT+CWLAP\r\n");
	while (strcmp(useLineBuffer(), "OK\r") != 0) {
		wifiBufferLoop();
		if (hasNewLine() && ssidCount < 5) {
			if (strstr(useLineBuffer(), "+CWLAP:") != NULL) {
				sscanf_P(useLineBuffer(),
				         PSTR("+CWLAP:(%d,\"%31[0-9a-zA-Z ]\",%d,\"%17[0-9a-f:]\",%d,%*d,%*d)\r"),
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