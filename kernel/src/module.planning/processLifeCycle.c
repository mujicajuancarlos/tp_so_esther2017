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

void sendToREADY(Process* process) {
	pthread_mutex_lock(&readyListMutex);
	queue_push(states->ready , process);
	pthread_mutex_unlock(&readyListMutex);
	logTrace("El proceso %d ingresó a la lista de READY", process->pid);
}

void sendToEXEC(Process* process) {
	pthread_mutex_lock(&executeListMutex);
	list_add(states->execute, process);
	pthread_mutex_unlock(&executeListMutex);
	logTrace("El proceso %d ingresó a la lista de EXECUTE", process->pid);
}

void sendToBLOCK(Process* process) {
	pthread_mutex_lock(&blockListMutex);
	queue_push(states->block, process);
	pthread_mutex_unlock(&blockListMutex);
	logTrace("El proceso %d ingresó a la lista de BLOCK", process->pid);
}

void sendToEXIT(Process* process) {
	pthread_mutex_lock(&exitListMutex);
	queue_push(states->exit, process);
	pthread_mutex_unlock(&exitListMutex);
	logTrace("El proceso %d ingresó a la lista de EXIT", process->pid);
}

void sendToNEW(Process* process) {
	pthread_mutex_lock(&newListMutex);
	queue_push(states->exit, process);
	pthread_mutex_unlock(&newListMutex);
	logTrace("El proceso %d ingresó a la lista de NEW", process->pid);
}

void initializeProcessLifeCycle() {

	logTrace("Inicializando t_planningStates del Planificador");
	states = malloc(sizeof(t_planningStates));

	states->execute = list_create();
	states->exit = queue_create();
	states->new = queue_create();
	states->ready = queue_create();
	states->block = queue_create();

	pthread_mutex_init(&executeListMutex, NULL);
	pthread_mutex_init(&exitListMutex, NULL);
	pthread_mutex_init(&newListMutex, NULL);
	pthread_mutex_init(&readyListMutex, NULL);
	pthread_mutex_init(&blockListMutex, NULL);
}

void destroyProcessLifeCycle() {

//	list_destroy_and_destroy_elements(states->execute, (void*) destroyPCB);
//	queue_destroy_and_destroy_elements(states->exit, (void*) destroyPCB);
//	queue_destroy_and_destroy_elements(states->new, (void*) destroyPCB);
//	queue_destroy_and_destroy_elements(states->ready, (void*) destroyPCB);
//	queue_destroy_and_destroy_elements(states->block, (void*) destroyPCB);

	pthread_mutex_destroy(&executeListMutex);
	pthread_mutex_destroy(&exitListMutex);
	pthread_mutex_destroy(&newListMutex);
	pthread_mutex_destroy(&readyListMutex);
	pthread_mutex_destroy(&blockListMutex);
}
