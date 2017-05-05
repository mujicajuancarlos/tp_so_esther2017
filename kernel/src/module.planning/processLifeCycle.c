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

void initializeProcessLifeCycle() {

	logTrace("Inicializando Estados del Planificador");
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
