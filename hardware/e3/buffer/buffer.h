#ifndef BUFFER_H
	#define BUFFER_H
	#include <avr/eeprom.h>

	#define BUFFER_ADDR (void*)239

	#define MAX_RECORDS 49

	#define ERR_EMPTY_BUFFER 1
	#define WRR_OVERFLOW 2

	struct record_s {
		float temperature;
		float humidity;
		float coLevel;
		float dustLevel;
	};

	struct buffer_s {
		unsigned char count;
		unsigned char head;
		unsigned char tail;
		struct record_s records[MAX_RECORDS];
	};

	void buffer_init();
	unsigned char get_buffer_count();
	unsigned char buffer_push(struct record_s);
	unsigned char buffer_pop(struct record_s* result);

#endif