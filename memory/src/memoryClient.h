/*
 * memoryClient.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MEMORYCLIENT_H_
#define MEMORYCLIENT_H_

#include "memoryStruct.h"

//estructura para representar una un nuevo cliente conectado a la memoria
typedef struct MemoryClient {
	int fileDescriptor;
	memory_struct* memoryStruct;
} MemoryClient;

MemoryClient* createMemoryClient(int socket, memory_struct* memoryStruct);

void destroyMemoryClient(MemoryClient* memoryClient);

#endif /* MEMORYCLIENT_H_ */
