/*
 * cpuRequests.c
 *
 *  Created on: 23/5/2017
 *      Author: utnso
 */

#include "cpuRequest.h"

void startProcessExecution(Process* selectedProcess, CPU* selectedCPU){
	sendToEXEC(selectedProcess);
	selectedCPU->pid = selectedProcess->pid;
	logInfo("Iniciando ejecucion de pid: %d en cpu: %d",selectedProcess->pid,selectedCPU->fileDescriptor);
	char* buffer = serialize_PCB(selectedProcess->pcb);
	uint32_t size = sizeOf_PCB(selectedProcess->pcb);
	Package* package =  createAndSendPackage(selectedCPU->fileDescriptor,COD_EXEC_NEW_PCB,size,buffer);
	free(buffer);
	destroyPackage(package);
}
