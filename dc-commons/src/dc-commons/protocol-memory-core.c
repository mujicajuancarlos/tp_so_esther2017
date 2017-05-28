/*
 * protocol-memory-core.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#include "protocol-memory-core.h"

char* serialize_t_PageBytes(t_PageBytes* object) {
	char *stream = malloc(sizeof_t_PageBytes(object));
	uint32_t offset = 0;

	serialize_and_copy_value(stream, &(object->pid), sizeof(uint32_t), &offset);
	serialize_and_copy_value(stream, &(object->pageNumber), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, &(object->offset), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, &(object->size), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, &(object->buffer),
			sizeof(char) * object->size, &offset);

	return stream;
}

t_PageBytes* deserialize_t_PageBytes(char* stream) {
	t_PageBytes* object = malloc(sizeof(t_PageBytes));
	uint32_t offset = 0;

	deserialize_and_copy_value(&(object->pid), stream, sizeof(uint32_t),
			&offset);
	deserialize_and_copy_value(&(object->pageNumber), stream, sizeof(uint32_t),
			&offset);
	deserialize_and_copy_value(&(object->offset), stream, sizeof(uint32_t),
			&offset);
	deserialize_and_copy_value(&(object->size), stream, sizeof(uint32_t),
			&offset);

	size_t sizeBuffer = (sizeof(char) * object->size);
	object->buffer = calloc(1, sizeBuffer);
	deserialize_and_copy_value(&(object->buffer), stream,
			sizeof(char) * object->size, &offset);

	return object;
}

size_t sizeof_t_PageBytes(t_PageBytes* object) {
	return (sizeof(uint32_t) * 4) + sizeof(char) * object->size;
}

t_PageBytes* create_t_PageBytes(uint32_t pid, uint32_t pageNumber, uint32_t offset, uint32_t size, char* buffer) {
	t_PageBytes* object = malloc(sizeof(t_PageBytes));
	object->pid = pid;
	object->pageNumber = pageNumber;
	object->offset = offset;
	object->size = (sizeof(char) * size);
	object->buffer = calloc(1, object->size);
	memcpy(object->buffer, buffer, object->size);
	return object;
}

void destroy_t_PageBytes(t_PageBytes* object) {
	if (object != NULL) {
		if (object->buffer != NULL)
			free(object->buffer);
		free(object);
	}
}
