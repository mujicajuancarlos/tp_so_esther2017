/*
 * cpu.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "cpu.h"


CPU* createCPU(int socket, kernel_struct* kernelStruct) {
	CPU* cpu = malloc(sizeof(CPU));
	cpu->fileDescriptor = socket;
	cpu->libre = true;
	cpu->process = NULL;
	cpu->kernelStruct = kernelStruct;
	return cpu;
}

void destroyCPU(CPU* cpu) {
	if (cpu != NULL) {
		cpu->process = NULL;
		cpu->kernelStruct = NULL;
		free(cpu);
	}
}
