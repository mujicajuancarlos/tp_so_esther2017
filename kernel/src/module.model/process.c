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
	newProcess->pcb = NULL;
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

void createPcbForNewProcess(Process* process, Package* sourceCodePackage) {

	logInfo("Generando la metadata para el proceso %d", process->pid);
	t_metadata_program* metadata = metadata_desde_literal(sourceCodePackage->stream);

	//el va despues del codigo, por lo tanto con esta cuenta puedo saber cual es la primer pagina del stack
	uint32_t stackFirstPage = sourceCodePackage->size / process->kernelStruct->pageSize;

	logInfo("Generando el pcb para el proceso %d", process->pid);
	PCB* pcb = create_new_PCB(process->pid, stackFirstPage, metadata);

	logInfo("Pcb asociado al proceso de pid %d", process->pid);
	process->pcb = pcb;
}


void printProcess(Process* proceso){
	//imprimo pid
		printf("%d", proceso->pid);

	//imprimo estado
		char* state = getProcessState(proceso);
		printf("%s", state);

	//imprimo exit code
		printf("%d", proceso->pcb->exit_code);
}



