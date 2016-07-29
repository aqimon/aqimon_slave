#include "wifiServer.h"

int linkID;
char httpMethod[8], httpPath[16], httpVer[9];

void wifiServerInit() {
	wifiExecute(PSTR("AT+CWMODE_CUR=2"));
	wifiExecute(PSTR("AT+CWSAP_DEF=\"MakerWeek\",\"makerweek\",5,4,2,0"));
	wifiExecute(PSTR("AT+CIPAP_DEF=\"192.168.69.1\",\"192.168.69.0\",\"255.255.255.0\""));
	wifiExecute(PSTR("AT+CWDHCP_DEF=1,1"));
	wifiExecute(PSTR("AT+CIPMUX=1"));
	wifiExecute(PSTR("AT+CIPSERVER=1,80"));
	lcdUpdateWifiStatus(WIFI_SERVER);
}

void wifiServerListener() {
	while (1) {
		wifiBufferLoop();
		if (hasNewLine()) {
			if (strstr_P(useLineBuffer(), PSTR("+IPD")) != NULL) {
				// now we parse the request
				Serial.println(F("Get something, parsing to see if it's http or not"));
				if (sscanf_P(useLineBuffer(), PSTR("+IPD,%d,%*d:%7s %15s %8s\r"),
				             &linkID,
				             httpMethod,
				             httpPath,
				             httpVer)!=4){
					Serial.println(httpMethod);
					Serial.println(httpPath);
					Serial.println(httpVer);
					wifiInitiateSend(linkID);
					wifiServerSendPGMP(http400, &stream);
					wifiEndSend();
					continue;
				}
				if ((!strcmp_P(httpVer, PSTR("HTTP/1.1"))) || (!strcmp_P(httpVer, PSTR("HTTP/1.0")))) {
					Serial.println(F("It's really a HTTP request"));
					while (strcmp_P(useLineBuffer(), PSTR("\r")) != 0)
						wifiBufferLoop();
					if (strcmp_P(httpPath, PSTR("/")) == 0) {
						Serial.println(F("Sending index page"));
						wifiInitiateSend(linkID);
						wifiServerSendIndex(&stream);
						wifiEndSend();
					} else if (strcmp_P(httpPath, PSTR("/save")) == 0 && strcmp_P(httpMethod, PSTR("POST")) == 0) {
						wifiServerSaveConfig();
					} else if (strcmp_P(httpPath, PSTR("/currSetting")) == 0) {
						Serial.println(F("Sending curr setting"));
						wifiInitiateSend(linkID);
						wifiServerSendPGMP(http200json, &stream);
						wifiServerSendConfig(&stream);
						wifiEndSend();
					} else if (strcmp_P(httpPath, PSTR("/restart"))==0){
						Serial.println(F("Soft restarting"));
						wifiInitiateSend(linkID);
						wifiServerSendPGMP(http200json, &stream);
						wifiServerSendPGMP(success, &stream);
						wifiEndSend();
						while (1); // just let the watchdog trigger itself
					} else {
						Serial.println(F("Sending 404"));
						wifiInitiateSend(linkID);
						wifiServerSendPGMP(http404, &stream);
						wifiEndSend();
					}
				} else {
					wifiInitiateSend(linkID);
					wifiServerSendPGMP(http400, &stream);
					wifiEndSend();
				}
			} else if (strcmp_P(useLineBuffer(), PSTR("ready\r"))==0){
				Serial.println(F("ESP unexpectedly reseted, initializing"));
				wifiInit();
				wifiServerInit();
			}
		}
	}
}

void wifiServerSaveConfig() {
	Serial.println(F("Saving settings"));
	int fail = 0;
	while (strcmp_P(useLineBuffer(), PSTR("begin")) != 0)
		wifiBufferLoop();
	while (1) {
		wifiBufferLoop();
		if (hasNewLine()) {
			if (strcmp_P(useLineBuffer(), PSTR("end")) == 0)
				break;
			if (!configParseRequest(useLineBuffer())) {
				Serial.println(F("fail:"));
				Serial.println(useLineBuffer());
				Serial.println(F("end"));
				wifiInitiateSend(linkID);
				wifiServerSendPGMP(http400, &stream);
				wifiEndSend();
				fail = 1;
				break;
			}
		}
	}
	if (fail == 0) {
		configWrite();
		Serial.println(linkID);
		wifiInitiateSend(linkID);
		wifiServerSendPGMP(http200json, &stream);
		wifiServerSendPGMP(success, &stream);
		wifiEndSend();
	}
}

void wifiServerSendIndex(FILE *stream) {
	size_t i;
	wifiServerSendPGMP(http200gzip, stream);
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