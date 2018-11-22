#ifndef SIM900CORE_H

	#define SIM900CORE_H

	#include <AltSoftSerial.h>
	#include <avr/pgmspace.h>
	#include <Arduino.h>
	#include <avr/wdt.h>
	#include "../const.h"
	#include "../config/config.h"
	#include "../lcd/lcd.h"

	#define SIM900_TX 9
	#define SIM900_RX 8
	#define SIM900_RST 12
	#define LINE_BUFFER_SIZE 192
	#define SIM900_BUFFER_SIZE 192
	#define TIMEOUT 20000
	
	extern AltSoftSerial sim900;
	extern FILE stream;
	extern FILE commandStream;

	void sim900Init();
	void sim900Loop();
	void sim900BufferLoop();
	unsigned char sim900Reset();
	
	unsigned char sim900ConnectToAP();
	unsigned char sim900Execute(char *command);
	unsigned char sim900Execute(PGM_P command);
	unsigned char sim900WaitForResult();
	unsigned char hasNewLine();

	int sim900StreamSend(char c, FILE *stream);
	int sim900StreamSendCommand(char c, FILE *stream);
	unsigned char sim900InitiateSend();
	unsigned char sim900EndSend();

	char* useLineBuffer();
#endif