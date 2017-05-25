/*
 * scheduler.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#include "scheduler.h"

void initializeScheduler(kernel_struct* kernelStruct){
	initializeProcessLifeCycle();
	initializeMultiprogrammingLevel(kernelStruct);
	initializeshortTermScheduler();
}

void destroyScheduler(){
	destroyProcessLifeCycle();
	destroyMultiprogrammingLevel();
	destroyshortTermScheduler();
}

int getAlgorithmIndex(char* name){
	if(strcmp(name,"RR")) return ROUND_ROBIN;
	if(strcmp(name,"FIFO")) return FIFO;
	return -1;
}

void execute_RoundRobin_scheduling(kernel_struct *kernel_struct, CPU* selectedCPU){
	Process* process = getReadyProcess_with_FIFO();
	//todo a la espera de la respuesta de los ayudates para tratar lo del quantum

	startProcessExecution(process, selectedCPU);
}

void execute_FIFO_scheduling(kernel_struct *kernel_struct, CPU* selectedCPU){
	Process* process = getReadyProcess_with_FIFO();

	startProcessExecution(process, selectedCPU);
}

Process* getReadyProcess_with_FIFO(){
	return popToREADY();
}
