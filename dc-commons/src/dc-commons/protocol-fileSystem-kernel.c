/*
 * protocol-fileSystem-kernel.c
 *
 *  Created on: 18/6/2017
 *      Author: utnso
 */

#include "protocol-fileSystem-kernel.h"

size_t sizeof_t_fileData(t_fileData* object) {
	return (sizeof(uint32_t) * 3) + sizeof(char) * object->dataSize
			+ sizeof(char) * object->pathSize;
}

char* serialize_t_fileData(t_fileData* object) {
	char *stream = malloc(sizeof_t_fileData(object));
	uint32_t offset = 0;

	serialize_and_copy_value(stream, &(object->pathSize), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, object->path,
			sizeof(char) * object->pathSize, &offset);

	serialize_and_copy_value(stream, &(object->offset), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, &(object->dataSize), sizeof(uint32_t),
			&offset);
	serialize_and_copy_value(stream, object->data,
			sizeof(char) * object->dataSize, &offset);
	return stream;
}

t_fileData* deserialize_t_fileData(char* stream) {
	t_fileData* object = malloc(sizeof(t_fileData));
	uint32_t offset = 0;

	deserialize_and_copy_value(&(object->pathSize), stream, sizeof(uint32_t),
			&offset);

	object->path = string_substring(stream, offset, object->pathSize);
	offset += object->pathSize;

	deserialize_and_copy_value(&(object->offset), stream, sizeof(uint32_t),
			&offset);
	deserialize_and_copy_value(&(object->dataSize), stream, sizeof(uint32_t),
			&offset);
	object->data = malloc(sizeof(char) * object->dataSize);
	deserialize_and_copy_value(object->data, stream,
			sizeof(char) * object->dataSize, &offset);
	return object;
}

t_fileData* create_t_fileData(char* path, uint32_t offset, uint32_t dataSize) {
	t_fileData* object = malloc(sizeof(t_fileData));
	object->pathSize = sizeof(char) * strlen(path);
	object->path = string_duplicate(path);
	object->offset = offset;
	object->dataSize = sizeof(char) * dataSize;
	object->data = malloc(object->dataSize);
	return object;
}

void destroy_t_fileData(t_fileData* object) {
	if (object != NULL) {
		if (object->path != NULL) {
			free(object->path);
			object->path = NULL;
		}
		if (object->data != NULL) {
			free(object->data);
			object->data = NULL;
		}
		free(object);
	}
}
