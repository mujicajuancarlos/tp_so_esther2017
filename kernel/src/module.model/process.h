/*
 * ansisopProcess.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <parser/metadata_program.h>
#include <commons/string.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/package.h>
#include "../module.planning/stateConverter.h"
#include "../module.memory/heapMetadata.h"
#include "../kernelStruct.h"
#include "fileDescriptor.h"


#define FIRST_PID 1000
#define NEXT_FD 3

typedef struct {
	int burst_Counter;
	int sysC_Counter;
	int allocateTimes_Counter;
	int allocateSize_Counter;
	int freeTimes_Counter;
	int freeSize_Counter;
} t_processCounter;

typedef struct Process {
	int fileDescriptor;	//file descriptor del socket de la consola
	int pid;	//PID del proceso que esta ejecutando actualmente
	int quantum;
	int nextFD;
	bool forceQuit;	//usado para la finalizacion forzada de procesos en ejecucion
	PCB* pcb;
	t_list* heapPages;
	t_list* files;
	kernel_struct* kernelStruct;
	int exit_code;
	t_processCounter* processCounters;
} Process;

Process* createProcess(int socket, kernel_struct* kernelStruct);

void destroyProcess(Process* process);

void freeProcessResources(Process* process);

void initializeCurrentPidMutex();

void destroyCurrentPidMutex();

int getNextPID();
int getNextFileDescriptorFor(Process* process);

void markForcedQuitProcess(Process* process);

void createPcbForNewProcess(Process* process, Package* package);

void replacePCB(Process* process, PCB* newPCB);

t_processFile* getProcessFile(Process* process, int fileDescriptor);

void addFile(Process* process, t_processFile* processFile);
void removeFile(Process* process, int fileDescriptor);

void removeAndDestroyFile(Process* process, t_processFile* file);

void printHeaderProcess();

void printProcess(Process* proceso, int stateIndex);

void printProcessFull(Process* proceso);

void initializeProcessCounters(t_processCounter* processCounters);

void incrementCounter(int counter, int quantity);

void notifyEndProcess(Process* process);

#endif /* PROCESS_H_ */
