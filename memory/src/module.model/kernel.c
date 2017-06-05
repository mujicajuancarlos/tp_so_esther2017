/*
 * kernel.c
 *
 *  Created on: 4/6/2017
 *      Author: utnso
 */

#include "kernel.h"

kernel* createKernel(MemoryClient* newClient) {
	kernel* kernel = malloc(sizeof(kernel));
	kernel->fileDescriptor = newClient->fileDescriptor;
	kernel->memoryStruct = newClient->memoryStruct;
	return kernel;
}

void destroyKernel(kernel* kernel) {
	if (kernel != NULL) {
		free(kernel);
	}
}
