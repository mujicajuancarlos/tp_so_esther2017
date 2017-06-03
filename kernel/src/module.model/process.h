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
#include <parser/metadata_program.h>
#include <commons/string.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/package.h>
#include "../kernelStruct.h"
#include "../commons/collections/list.h"
#include "fileDescriptor.h"

#define FIRST_PID 1000

typedef struct Process {
    int fileDescriptor;	//file descriptor del socket de la consola
    int pid;	//PID del proceso que esta ejecutando actualmente
    int quantum;
    PCB* pcb;
    kernel_struct* kernelStruct;
    t_list* fileDescriptorList;
    int exit_code;
} Process;

t_list* tablaProcesosFD;

Process* createProcess(int socket,kernel_struct* kernelStruct);

void destroyProcess(Process* process);

void initializeCurrentPidMutex();

void destroyCurrentPidMutex();

int getNextPID();

void createPcbForNewProcess(Process* process, Package* package);

void replacePCB(Process* process, PCB* newPCB);

void printHeaderProcess();

void printProcess(Process* proceso, char* state);

void printProcessFull(Process* proceso);

void agregarPFD_Alista(t_processFileDescriptor* pfd);

void removerPFD_Lista(t_processFileDescriptor* pfd);

flags habilitarPermisos(char* permiso);

void destroy_t_processFileDescriptor(t_processFileDescriptor* pfd);

t_processFileDescriptor* createNew_t_processFileDescriptor(char* permiso,
		t_fileDescriptor* fd);

#endif /* PROCESS_H_ */
