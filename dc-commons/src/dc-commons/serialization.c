/*
 * serialization.c
 *
 *  Created on: 24/4/2017
 *      Author: utnso
 */

#include "serialization.h"

/**
 * Serializador generico
 */
void serialize_and_copy_value(char* buffer, void* value, size_t size_to_copy,
		uint32_t* offset) {
	memcpy(buffer + *offset, value, size_to_copy);
	*offset += size_to_copy;
}

/**
 * Deserializador generico
 */
void deserialize_and_copy_value(void* value, char* buffer,
		size_t size_to_copy, uint32_t* offset) {
	memcpy(value, buffer + *offset, size_to_copy);
	*offset += size_to_copy;
}

char* serialize_int(int value) {
	size_t size = sizeof(int);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

int deserialize_int(char* stream) {
	size_t size = sizeof(int);
	int value;
	memcpy(&value, stream, size);
	return value;
}

char* serialize_bool(bool value) {
	size_t size = sizeof(bool);
	char *stream = calloc(1, size);
	memcpy(stream, &value, size);
	return stream;
}

bool deserialize_bool(char* stream) {
	size_t size = sizeof(bool);
	bool value;
	memcpy(&value, stream, size);
	return value;
}

char* deserialize_string(char* stream, size_t length) {
	return string_substring_until(stream, length);
}
