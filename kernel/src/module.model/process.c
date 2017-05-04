/*
 * ansisopProcess.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#include "process.h"

int currentPid = FIRST_PID;
pthread_mutex_t currentPidMutex;

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

void initializeCurrentPidMutex(){
	pthread_mutex_init(&currentPidMutex,NULL);
}

void destroyCurrentPidMutex(){
	pthread_mutex_destroy(&currentPidMutex);
}

int getNextPID(){
	int next;
	pthread_mutex_lock(&currentPidMutex);
		next = currentPid++;
	pthread_mutex_unlock(&currentPidMutex);
	return next;
}
