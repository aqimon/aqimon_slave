#include "buffer.h"

struct buffer_s buffer;

void buffer_init(){
	eeprom_read_block((void*)(&buffer), BUFFER_ADDR, sizeof(struct buffer_s));
}

unsigned char get_buffer_count(){
	return buffer.count;
}

unsigned char buffer_pop(struct record_s* record){
	if (buffer.count == 0){
		return ERR_EMPTY_BUFFER;
	}
	*record = buffer.records[buffer.tail];
	--(buffer.count);
	buffer.tail = (buffer.tail - 1 + MAX_RECORDS) % MAX_RECORDS;
	eeprom_update_block(BUFFER_ADDR, (void*)(&buffer), sizeof(struct buffer_s));
	return 1;
}

unsigned char buffer_push(struct record_s* record){
	unsigned char result;
	buffer.tail = (buffer.tail + 1) % MAX_RECORDS;
	buffer.records[buffer.tail] = *record;
	if (buffer.tail == buffer.head){
		// overflow overflow overflow
		buffer.head = (buffer.head + 1) % MAX_RECORDS;
		result = WRR_OVERFLOW;
	} else {
		buffer.count++;
		result = 0;
	}
	eeprom_update_block(BUFFER_ADDR, (void*)(&buffer), sizeof(struct buffer_s));
	return result;
}