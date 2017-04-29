/*
 * cpu.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "cpu.h"


CPU* createCPU(int socket) {
	CPU* cpu = malloc(sizeof(CPU));
	cpu->fileDescriptor = socket;
	cpu->libre = true;
	cpu->pid = 0;
	return cpu;
}

void destroyCPU(CPU* cpu) {
	if (cpu != NULL) {
		free(cpu);
	}
}
