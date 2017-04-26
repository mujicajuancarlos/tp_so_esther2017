/*
 * ansisopProcess.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef ANSISOPPROCESS_H_
#define ANSISOPPROCESS_H_

#include <stdlib.h>

typedef struct Process {
    int fileDescriptor;	//file descriptor del socket del cpu
    int pid;	//PID del proceso que esta ejecutando actualmente
} Process;

Process* createProcess(int socket);

void destroyProcess(Process* process);

#endif /* ANSISOPPROCESS_H_ */
