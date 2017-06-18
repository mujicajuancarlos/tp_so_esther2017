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
	newProcess->pcb = NULL;
	newProcess->forceQuit = false;
	newProcess->nextFD = NEXT_FD;
	newProcess->heapPages = list_create();
	newProcess->files = list_create();
	newProcess->kernelStruct = kernelStruct;
	return newProcess;
}

void destroyProcess(Process* process) {
	if (process != NULL) {
		if (process->pcb != NULL)
			destroy_PBC(process->pcb);
		if (process->heapPages != NULL) {
			list_destroy_and_destroy_elements(process->heapPages,
					(void*) destroy_heap_page);
			process->heapPages = NULL;
		}
		if (process->files != NULL) {
			list_destroy_and_destroy_elements(process->files,
					(void*) destroy_t_processFile);
			process->files = NULL;
		}
		free(process);
	}
}

void freeProcessResources(Process* process) {
	process->fileDescriptor = -1;
	destroy_stackArray(process->pcb->stackIndex, process->pcb->stackSize);
	metadata_destruir(process->pcb->metadata);
	process->pcb->stackIndex = NULL;
	process->pcb->metadata = NULL;
	//todo: enviar solicitud a la memoria para que libere las paginas -> considerar tmb heaps
	//todo: enviar solicitud al file system para que libere los archivos
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

int getNextFileDescriptorFor(Process* process){
	int aux = process->nextFD;
	process->nextFD = aux + 1;
	return aux;
}

void markForcedQuitProcess(Process* process) {
	process->forceQuit = true;
}

void createPcbForNewProcess(Process* process, Package* sourceCodePackage) {

	logInfo("Generando la metadata para el proceso %d", process->pid);
	t_metadata_program* metadata = metadata_desde_literal(
			sourceCodePackage->stream);

	//el va despues del codigo, por lo tanto con esta cuenta puedo saber cual es la primer pagina del stack
	uint32_t stackFirstPage = sourceCodePackage->size
			/ process->kernelStruct->pageSize;

	logInfo("Generando el pcb para el proceso %d", process->pid);
	PCB* pcb = create_new_PCB(process->pid, stackFirstPage, metadata);

	logInfo("Pcb asociado al proceso de pid %d", process->pid);
	process->pcb = pcb;
}

void replacePCB(Process* process, PCB* newPCB) {
	PCB* oldPcb = process->pcb;
	destroy_PBC(oldPcb);
	process->pcb = newPCB;
}

void addFile(Process* process, t_processFile* processFile){
	list_add(process->files,process);
}

void removeFile(Process* process, int fileDescriptor){
	bool condition(void* element){
		t_processFile* file = element;
		return file->fd == fileDescriptor;
	}
	list_remove_by_condition(process->files,condition);
}

void printHeaderProcess() {
	printf("\n%5s\t%20s\t%20s\n%s\n", "PID", "STATE", "EXIT CODE",
			"===============================================================");
}

void printProcess(Process* proceso, int stateIndex) {
	char* exitCode =
			(proceso->pcb == NULL) ? "" : string_itoa(proceso->pcb->exit_code);
	char* state = stateIndexToString(stateIndex);
	printf("%5d\t%20s\t%20s\n", proceso->pid, state, exitCode);
}

void printProcessFull(Process* proceso, int stateIndex) {
	printf("\nInformación del proceso %d\n", proceso->pid);
	printf("\nEstado %s", stateIndexToString(stateIndex));
	printf("\tCantidad rafagas ejecutadas %d\n", 567);//TODO: completar rafagas ejecutadas
	printf("\tCantidad de operaciones privilegiadas %d\n", 142);//TODO: completar
	printf("\tTabla de archivos abiertos %d\n", 423);	//TODO: completar
	printf("\tCantidad de paginas heap utilizadas %d\n", 423);//TODO: completar
	printf("\tOtra cantidad %d\n", 432);	//TODO: completar
	printf("\tOtra cantidad %d\n", 234);	//TODO: completar
	printf("\tCantidad de syscall ejecutadas %d\n", 432);	//TODO: completar
}
