#ifndef WIFICLIENT_H

	#define WIFICLIENT_H

	#include "wifiCore.h"
	
	void wifiClientInit();
	unsigned char wifiConnectToAP();
	unsigned char wifiSendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel);
#endif