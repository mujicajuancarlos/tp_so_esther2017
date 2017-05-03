/*
 * ansisopProcess.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdlib.h>
#include "../kernelStruct.h"

typedef struct Process {
    int fileDescriptor;	//file descriptor del socket del cpu
    int pid;	//PID del proceso que esta ejecutando actualmente
    kernel_struct* kernelStruct;
} Process;

Process* createProcess(int socket,kernel_struct* kernelStruct);

void destroyProcess(Process* process);

#endif /* PROCESS_H_ */
