/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"

int currentPid = FIRST_PID;
pthread_mutex_t currentPidMutex;

Process* createProcess(int socket, kernel_struct* kernelStruct) {
	Process* newProcess = malloc(sizeof(Process));
	newProcess->fileDescriptor = socket;
	newProcess->pid = 0;
	newProcess->kernelStruct = kernelStruct;
	return newProcess;
}

void destroyProcess(Process* process) {
	if (process != NULL) {
		if(process->pcb != NULL)
			destroy_PBC(process->pcb);
		free(process);
	}
}

void initializeCurrentPidMutex() {
	pthread_mutex_init(&currentPidMutex, NULL);
}

void destroyCurrentPidMutex() {
	pthread_mutex_destroy(&currentPidMutex);
}

int getNextPID() {
	int next;
	pthread_mutex_lock(&currentPidMutex);
	next = currentPid++;
	pthread_mutex_unlock(&currentPidMutex);
	return next;
}

void createPcbForNewProcess(Process* process, Package* package) {

	logInfo("Generando la metadata para el proceso %d", process->pid);
	t_metadata_program* metadata = metadata_desde_literal(package->stream);

	uint32_t stackFirstPage = 1; //todo este dato se obtiene en base a los datos que me devuelve la memoria

	PCB* pcb = create_new_PCB(process->pid, stackFirstPage, metadata);

	process->pcb = pcb;
}
