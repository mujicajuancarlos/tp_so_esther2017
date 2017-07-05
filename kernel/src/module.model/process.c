/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"
#include "../module.memory/memoryRequests.h"

int currentPid = FIRST_PID;
pthread_mutex_t currentPidMutex;

Process* createProcess(int socket, kernel_struct* kernelStruct) {
	Process* newProcess = malloc(sizeof(Process));
	newProcess->fileDescriptor = socket;
	newProcess->pid = 0;
	newProcess->pcb = NULL;
	newProcess->forceQuit = false;
	newProcess->aborted = false;
	newProcess->nextFD = NEXT_FD;
	newProcess->heapPages = list_create();
	newProcess->files = list_create();
	newProcess->kernelStruct = kernelStruct;
	newProcess->exit_code = -1;
	newProcess->processCounters = malloc(sizeof(t_processCounter));
	initializeProcessCounters(newProcess->processCounters);
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
		if (process->processCounters != NULL) {
			free(process->processCounters);
			process->processCounters = NULL;
		}
		free(process);
	}
}

void setForceExitCode(Process* process) {
	if (process->fileDescriptor == -1) {
		process->exit_code = SC_ERROR_END_PROCESS_BY_DISCONECT;
	} else {
		process->exit_code = SC_ERROR_END_PROCESS_BY_REQUEST;
	}
}
/**
 * liberar recursos para un proceso que finalizo la ejecucion o fue stopeado
 */
void freeProcessResources(Process* process) {
	if (process->pcb != NULL) {
		destroy_PBC(process->pcb);
	}
	if (process->heapPages != NULL) {
		freeProcessHeapPagesResources(process);
	}
	if (process->files != NULL) {
		freeProcessFilesResources(process);
	}
	notifyEndProcess(process);
	close(process->fileDescriptor);
}

void freeProcessHeapPagesResources(Process* process) {
	void freeHeapPageResources(void* element) {
		heap_page* page = element;
		if (page->metadataList != NULL) {
			int status;
			list_destroy_and_destroy_elements(page->metadataList,
					(void*) destroy_heap_metadata);
			page->metadataList = NULL;
			freePageForProcess(process, page, &status);
		}
	}
	list_iterate(process->heapPages, freeHeapPageResources);
	list_destroy_and_destroy_elements(process->heapPages,
			(void*) destroy_heap_page);
	process->heapPages = NULL;
}

void freeProcessFilesResources(Process* process) {
	list_destroy_and_destroy_elements(process->files,
			(void*) destroy_t_processFile);
	process->files = NULL;
	removeClosedGlobalFiles();
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

int getNextFileDescriptorFor(Process* process) {
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

	uint32_t stackFirstPage =
			(sourceCodePackage->size % process->kernelStruct->pageSize) ? 1 : 0;
	stackFirstPage += sourceCodePackage->size / process->kernelStruct->pageSize;

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

t_processFile* getProcessFile(Process* process, int fileDescriptor) {
	bool condition(void* element) {
		t_processFile* file = element;
		return file->fd == fileDescriptor;
	}
	return list_find(process->files, condition);
}

void addFile(Process* process, t_processFile* processFile) {
	list_add(process->files, processFile);
}

void removeFile(Process* process, int fileDescriptor) {
	bool condition(void* element) {
		t_processFile* file = element;
		return file->fd == fileDescriptor;
	}
	list_remove_by_condition(process->files, condition);
}

void removeAndDestroyFile(Process* process, t_processFile* file) {
	removeFile(process, file->fd);
	destroy_t_processFile(file);
	removeClosedGlobalFiles();
}

void printHeaderProcess() {
	printf("\n%5s\t%20s\t%20s\n%s\n", "PID", "STATE", "EXIT CODE",
			"===============================================================");
}

void printProcess(Process* proceso, int stateIndex) {
	char* exitCode =
			(proceso->exit_code == -1) ? "" : string_itoa(proceso->exit_code);
	char* state = stateIndexToString(stateIndex);
	printf("%5d\t%20s\t%20s\n", proceso->pid, state, exitCode);
}

void printProcessFull(Process* proceso) {
	printf("\nInformación del proceso %d\n", proceso->pid);
//	printf("\nEstado %s", stateIndexToString(getProcessStateIndex(proceso)));
	printf("\tCantidad rafagas ejecutadas %d\n",
			proceso->processCounters->burst_Counter);
	printf("\tCantidad de operaciones privilegiadas o syscalls: %d\n",
			proceso->processCounters->sysC_Counter);
	if (proceso->files != NULL) {
		printf("\tTabla de archivos abiertos: %d\n",
				proceso->files->elements_count);
		if (proceso->files->elements_count > 0) {
			printf("\t%5s %5s %5s %5s %s \n", "FD", "Seek", "Read",
					"Write", "Name");
			void printProcessFile(void* element) {
				t_processFile* file = element;
				printf("\t%5d %5d %5s %5s %s \n", file->fd,
						file->seekValue, file->flag.read ? "true" : "false",
						file->flag.write ? "true" : "false",
						file->globalFile->path);
			}
			list_iterate(proceso->files, printProcessFile);
		}

	} else {
		printf("\tTabla de archivos abiertos: 0\n");
	}

	if(proceso->heapPages!=NULL){
		printf("\tCantidad de paginas heap utilizadas: %d\n",proceso->heapPages->elements_count);
	}else {
		printf("\tCantidad de paginas heap utilizadas: 0\n");
	}

	printf("\tCantidad de operaciones alocar: %d en Bytes: %d\n",
			proceso->processCounters->allocateTimes_Counter,
			proceso->processCounters->allocateSize_Counter);
	printf("\tCantidad de operaciones liberar: %d en Bytes: %d\n",
			proceso->processCounters->freeTimes_Counter,
			proceso->processCounters->freeSize_Counter);
}

void initializeProcessCounters(t_processCounter* processCounters) {
	processCounters->allocateSize_Counter = 0;
	processCounters->allocateTimes_Counter = 0;
	processCounters->burst_Counter = 0;
	processCounters->freeSize_Counter = 0;
	processCounters->freeTimes_Counter = 0;
	processCounters->sysC_Counter = 0;
}

void incrementCounter(int* counter, int quantity) {
	*counter += quantity;
}
