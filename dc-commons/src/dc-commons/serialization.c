/*
 * serialization.c
 *
 *  Created on: 24/4/2017
 *      Author: utnso
 */

#include "serialization.h"


char *serialize_int(int value){
	size_t size = sizeof(int);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

int deserialize_int(char* stream){
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}

char* deserialize_string(char* stream, size_t length){
	return string_substring_until(stream, length);
}
