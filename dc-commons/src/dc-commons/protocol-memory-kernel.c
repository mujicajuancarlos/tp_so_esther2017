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
size_t sizeof_t_GetPageBytes(){
	return (sizeof(uint32_t) * 4);
}

t_AddPagesToProcess* create_t_AddPagesToProcess(int pid, int size){
	t_AddPagesToProcess* object = malloc(sizeof_t_AddPagesToProcess());
	object->pid = pid;
	object->size = size;
	return object;
}

char* serialize_t_SetPageBytes(t_SetPageBytes* object) {
	char *stream = malloc(sizeof_t_SetPageBytes(object));
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

t_SetPageBytes* deserialize_t_SetPageBytes(char* stream) {
	t_SetPageBytes* object = malloc(sizeof(t_SetPageBytes));
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

size_t sizeof_t_SetPageBytes(t_SetPageBytes* object) {
	return (sizeof(uint32_t) * 4) + sizeof(char) * object->size;
}

t_SetPageBytes* create_t_SetPageBytes(uint32_t size, char* buffer) {
	t_SetPageBytes* object = malloc(sizeof(t_SetPageBytes));
	object->size = size;
	size_t sizeBuffer = (sizeof(char) * size);
	object->buffer = calloc(1, sizeBuffer);
	memcpy(object->buffer, buffer, sizeBuffer);
	return object;
}

void destroy_t_SetPageBytes(t_SetPageBytes* object) {
	if (object != NULL) {
		if (object->buffer != NULL)
			free(object->buffer);
		free(object);
	}
}
