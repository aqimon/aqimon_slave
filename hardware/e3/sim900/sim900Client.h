#ifndef SIM900CLIENT_H

	#define SIM900CLIENT_H

	#include "sim900Core.h"
	
	void sim900ClientInit();
	unsigned char sim900ConnectToAPN();
	unsigned char sim900SendHTTPRequest(float temperature, float humidity, float dustLevel, float coLevel);
#endif