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
		if (process->heapPages != NULL) {
			freeProcessHeapPagesResources(process);
		}
		if (process->files != NULL) {
			freeProcessFilesResources(process);
		}
	}else{
		list_destroy(process->heapPages);
		list_destroy(process->files);
	}
	notifyEndProcessToConsole(process);
	notifyEndProcessToMemory(process);
}

void freeProcessHeapPagesResources(Process* process) {
	int startHeap = process->pcb->stackFirstPage + process->pcb->stackSize;
	void freeHeapPageResources(void* element) {
		heap_page* page = element;
		if (page->metadataList != NULL) {
			int status;
			list_destroy_and_destroy_elements(page->metadataList,
					(void*) destroy_heap_metadata);
			page->metadataList = NULL;
			int pageNumber = startHeap + page->page;
			freePageForProcess(process, pageNumber, &status);
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

/**
 * retorna el numero de pagina inicial para heap
 */
int getStartHeapPageNumber(Process* process){
	return process->pcb->stackFirstPage + process->kernelStruct->config->stack_size;
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

void printProcessCounters(Process* proceso) {
	printf("\tCantidad rafagas ejecutadas %d\n",
			proceso->processCounters->burst_Counter);
	printf("\tCantidad de operaciones privilegiadas o syscalls: %d\n",
			proceso->processCounters->sysC_Counter);
}

void printProcessFiles(Process* proceso) {
	if (proceso->files != NULL) {
		printf("\tTabla de archivos abiertos: %d\n",
				proceso->files->elements_count);
		if (proceso->files->elements_count > 0) {
			printf("\t%5s %5s %5s %5s %s \n", "FD", "Seek", "Read", "Write",
					"Name");
			void printProcessFile(void* element) {
				t_processFile* file = element;
				printf("\t%5d %5d %5s %5s %s \n", file->fd, file->seekValue,
						file->flag.read ? "true" : "false",
						file->flag.write ? "true" : "false",
						file->globalFile->path);
			}
			list_iterate(proceso->files, printProcessFile);
		}
	} else {
		printf("\tTabla de archivos abiertos: 0\n");
	}
}

void printProcessHeaps(Process* process) {
	if (process->heapPages != NULL) {
		printf("\tCantidad de paginas heap utilizadas: %d\n",
				process->heapPages->elements_count);
	} else {
		printf("\tCantidad de paginas heap utilizadas: 0\n");
	}
}

void printProcessHeapCounters(Process* process) {
	printf("\tCantidad de operaciones alocar: %d en Bytes: %d\n",
			process->processCounters->allocateTimes_Counter,
			process->processCounters->allocateSize_Counter);
	printf("\tCantidad de operaciones liberar: %d en Bytes: %d\n",
			process->processCounters->freeTimes_Counter,
			process->processCounters->freeSize_Counter);
}

void printProcessFull(Process* process) {
	printf("\nInformación del proceso %d\n", process->pid);
	printProcessCounters(process);
	printProcessFiles(process);
	printProcessHeaps(process);
	printProcessHeapCounters(process);
}

void printQuitStatusToProcess(Process* process) {
	printf(">> El proceso %d finalizó exitosamente\n", process->pid);
	printProcessCounters(process);
	printProcessHeapCounters(process);
	if (process->files != NULL && process->files->elements_count > 0) {
		printf("[Advertencia!] El proceso no cerró los siguientes archivos\n");
		printProcessFiles(process);
	}
	bool condition(void* element) {
		heap_page* page = element;
		return !page->isGarbage;
	}
	if (process->heapPages != NULL) {
		t_list* sublist = list_filter(process->heapPages, condition);
		if (sublist->elements_count > 0) {
			printf(
					"[Advertencia!] El proceso no liberó los siguientes bloques de memoria\n");
			printf("\t%5s %6s %5s\n", "Page", "Offset", "Size");
			void printNotFreePage(void* element) {
				heap_page* page = element;
				void printNotFreeBlock(void* blockElement) {
					heap_metadata* block = blockElement;
					if (!block->isFree) {
						printf("\t%5d %6d %5d\n", page->page, block->dataOffset,
								block->dataSize);
					}
				}
				list_iterate(page->metadataList, printNotFreeBlock);
			}
			list_iterate(sublist, printNotFreePage);
		}
		list_destroy(sublist);
	}
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
