/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"

Process* createProcess(int socket) {
	Process* cpu = malloc(sizeof(Process));
	cpu->fileDescriptor = socket;
	cpu->pid = 0;
	return cpu;
}

void destroyProcess(Process* process) {
	if (process != NULL) {
		free(process);
	}
}
