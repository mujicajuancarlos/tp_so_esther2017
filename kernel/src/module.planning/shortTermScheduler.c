/*
 * shortTermScheduler.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#include "shortTermScheduler.h"

pthread_mutex_t shortTermSchedulerMutex;

/**
 * incremento cuando: agregan un elemento a la lista ready
 * decremento cuando:
 * 		- el planificador lo toma para ejecutar
 * 		- las consolas solicitan finalizar un proceso que esta en ready
 */
sem_t processInReady_sem;

void initializeshortTermScheduler() {
	pthread_mutex_init(&shortTermSchedulerMutex, NULL);
	sem_init(&processInReady_sem, 1, 0);
}

void destroyshortTermScheduler() {
	pthread_mutex_destroy(&shortTermSchedulerMutex);
	sem_destroy(&processInReady_sem);
}

void shortTermScheduler_lock() {
	pthread_mutex_lock(&shortTermSchedulerMutex);
	logInfo("El planificador de corto plazo fue bloqueado");
}

void shortTermScheduler_unlock() {
	pthread_mutex_unlock(&shortTermSchedulerMutex);
	logInfo("El planificador de corto plazo fue desbloqueado");
}

void processInReady_wait() {
	sem_wait(&processInReady_sem);
	logInfo("Se decremento (wait) el recurso de 'Procesos en ready'");
}

void processInReady_signal() {
	logInfo("Se incremento (signal) el recurso de 'Procesos en ready'");
	sem_post(&processInReady_sem);
}
