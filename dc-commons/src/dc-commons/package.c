/*
 * package.c
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#include "package.h"

Package* createPackage(uint32_t msgCode, uint32_t size, char* stream) {
	Package *package = malloc(sizeof(Package));
	package->stream = malloc(sizeof(char) * size);
	memcpy(package->stream, stream, size);
	package->size = size;
	package->msgCode = msgCode;
	return package;
}

Package* createEmptyPackage() {
	Package *package = malloc(sizeof(Package));
	package->stream = NULL;
	package->size = 0;
	package->msgCode = 0;
	return package;
}

size_t sizePackage(Package *package) {
	return sizeof(uint32_t) + sizeof(uint32_t)
			+ (sizeof(char) * package->size);
}

void destroyPackage(Package* package) {
	if (package != NULL) {
		if(package->stream != NULL)
			free(package->stream);
		free(package);
	}
}

char* serializePackage(Package *package){

	char *serializedPackage = calloc(1,sizePackage(package));

	int offset = 0;
	size_t size_to_send;

	size_to_send =  sizeof(uint32_t);
	memcpy(serializedPackage + offset, &(package->msgCode), size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(uint32_t);
	memcpy(serializedPackage + offset, &(package->size), size_to_send);
	offset += size_to_send;

	if(package->size > 0){
		size_to_send = sizeof(char) * package->size;
		memcpy(serializedPackage + offset, package->stream, size_to_send);
	}

	return serializedPackage;
}
