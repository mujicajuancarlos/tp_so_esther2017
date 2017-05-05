/*
 * protocol-memory-kernel.c
 *
 *  Created on: 4/5/2017
 *      Author: utnso
 */

#include "protocol-memory-kernel.h"

/**
 * funciones recervar paginas para un nuevo proceso
 */
t_new_sourceCode_request* create_new_sourceCode_request(uint32_t pid,
		uint32_t stackSize, uint32_t sourceCodeSize, char* sourceCode) {
	t_new_sourceCode_request* request = calloc(1,
			sizeof(t_new_sourceCode_request));
	size_t sizeBuffer = (sizeof(char) * sourceCodeSize);
	request->sourceCode = calloc(1, sizeBuffer);
	memcpy(request->sourceCode, sourceCode, sizeBuffer);
	request->pid = pid;
	request->stackSize = stackSize;
	request->sourceCodeSize = sourceCodeSize;
	return request;
}
void destroy_new_sourceCode_request(t_new_sourceCode_request* request) {
	if (request != NULL) {
		if (request->sourceCode != NULL)
			free(request->sourceCode);
		free(request);
	}
}
size_t size_new_sourceCode_request(t_new_sourceCode_request* request) {
	size_t total;
	total = sizeof(uint32_t) * 3;
	if (request->sourceCode != NULL)
		total += sizeof(char) * request->sourceCodeSize;
	return total;
}
char* serialize_new_sourceCode_request(t_new_sourceCode_request* request) {
	char *stream = calloc(1, size_new_sourceCode_request(request));

	int offset = 0;
	size_t size_to_copy = sizeof(uint32_t);

	memcpy(stream + offset, &(request->pid), size_to_copy);
	offset += size_to_copy;

	memcpy(stream + offset, &(request->stackSize), size_to_copy);
	offset += size_to_copy;

	memcpy(stream + offset, &(request->sourceCodeSize), size_to_copy);
	offset += size_to_copy;

	if (request->sourceCodeSize > 0) {
		size_to_copy = sizeof(char) * request->sourceCodeSize;
		memcpy(stream + offset, request->sourceCode, size_to_copy);
	}

	return stream;
}
t_new_sourceCode_request* deserialize_new_sourceCode_request(char* stream) {

	t_new_sourceCode_request* request = calloc(1,
			sizeof(t_new_sourceCode_request));
	int offset = 0;
	size_t size_to_copy = sizeof(uint32_t);

	//char *buffer = malloc(buffer_size);

	memcpy(&(request->pid), stream + offset, size_to_copy);
	offset += size_to_copy;

	memcpy(&(request->stackSize), stream + offset, size_to_copy);
	offset += size_to_copy;

	memcpy(&(request->sourceCodeSize), stream + offset, size_to_copy);
	offset += size_to_copy;

	if (request->sourceCodeSize > 0) {
		size_to_copy = sizeof(char) * request->sourceCodeSize;
		request->sourceCode = calloc(1, size_to_copy);
		memcpy(request->sourceCode, stream + offset, size_to_copy);
	}

	return request;
}
