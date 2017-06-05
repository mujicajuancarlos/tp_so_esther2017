/*
 * cpu.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "cpu.h"

CPU* createCPU(MemoryClient* newClient) {
	CPU* cpu = malloc(sizeof(CPU));
	cpu->fileDescriptor = newClient->fileDescriptor;
	cpu->memoryStruct = newClient->memoryStruct;
	return cpu;
}

void destroyCPU(CPU* cpu) {
	if (cpu != NULL) {
		free(cpu);
	}
}
