/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"

Process* createProcess(int socket,kernel_struct* kernelStruct) {
	Process* newProcess = malloc(sizeof(Process));
	newProcess->fileDescriptor = socket;
	newProcess->pid = 0;
	newProcess->kernelStruct = kernelStruct;
	return newProcess;
}

void destroyProcess(Process* process) {
	if (process != NULL) {
		free(process);
	}
}
