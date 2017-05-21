/*
 * processLifeCycle.c
 *
 *  Created on: 5/5/2017
 *      Author: utnso
 */

#include "processLifeCycle.h"

pthread_mutex_t executeListMutex;
pthread_mutex_t exitListMutex;
pthread_mutex_t newListMutex;
pthread_mutex_t readyListMutex;
pthread_mutex_t blockListMutex;

t_planningStates* states;

void moveFromNewToReady(Process* process) {
	removeFromNEW(process);
	sendToREADY(process);
}

void moveFromExcecToReady(Process* process) {
	removeFromEXEC(process);
	sendToREADY(process);
}

void moveFromExcecToExit(Process* process) {
	removeFromEXEC(process);
	sendToEXEC(process);
}

void moveFromExcecToBlock(Process* process) {
	removeFromEXEC(process);
	sendToBLOCK(process);
}

void sendToREADY(Process* process) {
	pthread_mutex_lock(&readyListMutex);
	queue_push(states->ready, process);
	pthread_mutex_unlock(&readyListMutex);
	logTrace("El proceso %d ingresó a la lista de READY", process->pid);
}

Process* popToREADY() {
	Process* process;
	pthread_mutex_lock(&readyListMutex);
	process = queue_pop(states->ready);
	pthread_mutex_unlock(&readyListMutex);
	logTrace("El proceso %d salio a la lista de READY", process->pid);
	return process;
}

void sendToEXEC(Process* process) {
	pthread_mutex_lock(&executeListMutex);
	list_add(states->execute, process);
	pthread_mutex_unlock(&executeListMutex);
	logTrace("El proceso %d ingresó a la lista de EXECUTE", process->pid);
}

void removeFromEXEC(Process* process) {
	bool condition(void* element) {
		Process* anyProcess = element;
		return anyProcess->pid == process->pid;
	}
	pthread_mutex_lock(&executeListMutex);
	list_remove_by_condition(states->execute, condition);
	pthread_mutex_unlock(&executeListMutex);
	logTrace("El proceso %d salio de la lista de EXECUTE", process->pid);
}

void sendToBLOCK(Process* process) {
	pthread_mutex_lock(&blockListMutex);
	queue_push(states->block, process);
	pthread_mutex_unlock(&blockListMutex);
	logTrace("El proceso %d ingresó a la lista de BLOCK", process->pid);
}

Process* popToBLOCK() {
	Process* process;
	pthread_mutex_lock(&blockListMutex);
	process = queue_pop(states->block);
	pthread_mutex_unlock(&blockListMutex);
	logTrace("El proceso %d salio a la lista de BLOCK", process->pid);
	return process;
}

void sendToEXIT(Process* process) {
	pthread_mutex_lock(&exitListMutex);
	queue_push(states->exit, process);
	pthread_mutex_unlock(&exitListMutex);
	logTrace("El proceso %d ingresó a la lista de EXIT", process->pid);
}

Process* popToEXIT() {
	Process* process;
	pthread_mutex_lock(&exitListMutex);
	process = queue_pop(states->exit);
	pthread_mutex_unlock(&exitListMutex);
	logTrace("El proceso %d salio a la lista de EXIT", process->pid);
	return process;
}

void sendToNEW(Process* process) {
	pthread_mutex_lock(&newListMutex);
	list_add(states->new, process);
	pthread_mutex_unlock(&newListMutex);
	logTrace("El proceso %d ingresó a la lista de NEW", process->pid);
}

void removeFromNEW(Process* process) {
	bool condition(void* element) {
		Process* anyProcess = element;
		return anyProcess->pid == process->pid;
	}
	pthread_mutex_lock(&newListMutex);
	list_remove_by_condition(states->new, condition);
	pthread_mutex_unlock(&newListMutex);
	logTrace("El proceso %d salio de la lista de EXECUTE", process->pid);
}

void initializeProcessLifeCycle() {

	logTrace("Inicializando t_planningStates del Planificador");
	states = malloc(sizeof(t_planningStates));

	states->execute = list_create();
	states->exit = queue_create();
	states->new = list_create();
	states->ready = queue_create();
	states->block = queue_create();

	pthread_mutex_init(&executeListMutex, NULL);
	pthread_mutex_init(&exitListMutex, NULL);
	pthread_mutex_init(&newListMutex, NULL);
	pthread_mutex_init(&readyListMutex, NULL);
	pthread_mutex_init(&blockListMutex, NULL);
}

void destroyProcessLifeCycle() {

	list_destroy_and_destroy_elements(states->execute, (void*) destroyProcess);
	queue_destroy_and_destroy_elements(states->exit, (void*) destroyProcess);
	list_destroy_and_destroy_elements(states->new, (void*) destroyProcess);
	queue_destroy_and_destroy_elements(states->ready, (void*) destroyProcess);
	queue_destroy_and_destroy_elements(states->block, (void*) destroyProcess);

	pthread_mutex_destroy(&executeListMutex);
	pthread_mutex_destroy(&exitListMutex);
	pthread_mutex_destroy(&newListMutex);
	pthread_mutex_destroy(&readyListMutex);
	pthread_mutex_destroy(&blockListMutex);
}
