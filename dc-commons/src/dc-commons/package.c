/*
 * package.c
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#include "package.h"

Package* createPackage(uint32_t msgCode, char* message, uint32_t message_long) {
	Package *package = malloc(sizeof(Package));
	package->message = malloc(sizeof(char) * message_long);
	memcpy(package->message, message, message_long);
	package->message_long = message_long;
	package->msgCode = msgCode;
	return package;
}

int sizePackage(Package *package) {
	return sizeof(package->msgCode) + sizeof(package->message_long)
			+ (sizeof(char) * package->message_long);
}

void destroyPackage(Package* package) {
	if (package != NULL) {
		free(package->message);
	}
	free(package);
}
