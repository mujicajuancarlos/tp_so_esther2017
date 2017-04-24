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
	package->msgCode = -1;
	return package;
}

int sizePackage(Package *package) {
	return sizeof(package->msgCode) + sizeof(package->size)
			+ (sizeof(char) * package->size);
}

void destroyPackage(Package* package) {
	if (package != NULL) {
		free(package->stream);
	}
	free(package);
}

char* serializePackage(Package *package){

	char *serializedPackage = calloc(1,sizePackage(package));

	int offset = 0;
	int size_to_send;

	size_to_send =  sizeof(package->msgCode);
	memcpy(serializedPackage + offset, &(package->msgCode), size_to_send);
	offset += size_to_send;

	size_to_send =  sizeof(package->size);
	memcpy(serializedPackage + offset, &(package->size), size_to_send);
	offset += size_to_send;

	size_to_send =  package->size;
	memcpy(serializedPackage + offset, package->stream, size_to_send);

	return serializedPackage;
}
