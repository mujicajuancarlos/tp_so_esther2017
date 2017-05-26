/*
 * protocol-memory-kernel.c
 *
 *  Created on: 4/5/2017
 *      Author: utnso
 */

#include "protocol-memory-kernel.h"

size_t sizeof_t_AddPagesToProcess(){
	return (sizeof(uint32_t) * 2);
}

t_AddPagesToProcess* create_t_AddPagesToProcess(int pid, int size){
	t_AddPagesToProcess* object = malloc(sizeof_t_AddPagesToProcess());
	object->pid = pid;
	object->size = size;
	return object;
}

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

t_PageBytes* create_t_PageBytes(uint32_t size, char* buffer) {
	t_PageBytes* object = malloc(sizeof(t_PageBytes));
	object->size = size;
	size_t sizeBuffer = (sizeof(char) * size);
	object->buffer = calloc(1, sizeBuffer);
	memcpy(object->buffer, buffer, sizeBuffer);
	return object;
}

void destroy_t_PageBytes(t_PageBytes* object) {
	if (object != NULL) {
		if (object->buffer != NULL)
			free(object->buffer);
		free(object);
	}
}
