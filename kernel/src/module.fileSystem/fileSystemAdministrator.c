/*
 * fileSystemAdministrator.c
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#include "fileSystemAdministrator.h"


size_t sizeOf_t_Read(t_Read* read) {
	size_t size = 0;
	size += sizeof(int);
	size += sizeof(read->size);
	size += sizeof(char) * read->size;
	return size;
}

size_t sizeOf_t_Write(t_Write* write) {
	size_t size = 0;
	size += sizeof(int);
	size += sizeof(write->size);
	size += 2 * (sizeof(char) * write->size);
	return size;
}

char* serialize_t_Read(t_Read* read) {
	char* buffer = malloc(sizeOf_t_Read(read));

	uint32_t offset = 0;

	serialize_and_copy_value(buffer, read->path, sizeof(char) * read->size,
			&offset);

	serialize_and_copy_value(buffer, &(read->offset), sizeof(int), &offset);

	serialize_and_copy_value(buffer, &(read->size), sizeof(size_t), &offset);

	return buffer;
}

char* serialize_t_Write(t_Write* write) {
	char* buffer = malloc(sizeOf_t_Write(write));

	uint32_t offset = 0;

	serialize_and_copy_value(buffer, write->path, sizeof(char) * write->size,
			&offset);

	serialize_and_copy_value(buffer, &(write->offset), sizeof(int), &offset);

	serialize_and_copy_value(buffer, &(write->size), sizeof(size_t), &offset);

	serialize_and_copy_value(buffer, write->buffer, sizeof(char) * write->size,
			&offset);

	return buffer;
}

t_Read* deserialize_t_Read(char* buffer) {

	t_Read* read = malloc(sizeof(t_Read));

	uint32_t offset = 0;

	read->path = malloc(sizeof(char) * read->size);

	deserialize_and_copy_value(read->path, buffer, sizeof(char) * read->size,
			&offset);

	deserialize_and_copy_value(&(read->offset), buffer, sizeof(int), &offset);

	deserialize_and_copy_value(&(read->size), buffer, sizeof(size_t), &offset);
	return read;
}

t_Write* deserialize_t_Write(char* buffer) {
	t_Write* write = malloc(sizeof(t_Write));
	uint32_t offset = 0;

	write->path = malloc(sizeof(char) * write->size);

	deserialize_and_copy_value(write->path, buffer, sizeof(char) * write->size,
			&offset);

	deserialize_and_copy_value(&(write->offset), buffer, sizeof(int), &offset);

	deserialize_and_copy_value(&(write->size), buffer, sizeof(size_t), &offset);

	write->buffer = malloc(sizeof(char) * write->size);

	deserialize_and_copy_value(write->buffer, buffer,
			sizeof(char) * write->size, &offset);

	return write;

}

