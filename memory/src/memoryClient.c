/*
 * memoryCliente.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "memoryClient.h"

MemoryClient* createMemoryClient(int socket, memory_struct* memoryStruct) {
	MemoryClient* memoryClient = malloc(sizeof(MemoryClient));
	memoryClient->fileDescriptor = socket;
	memoryClient->memoryStruct = memoryStruct;
	return memoryClient;
}

void destroyMemoryClient(MemoryClient* memoryClient) {
	if (memoryClient != NULL) {
		free(memoryClient);
	}
}
