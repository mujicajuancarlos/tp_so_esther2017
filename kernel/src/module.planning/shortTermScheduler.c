/*
 * shortTermScheduler.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#include "shortTermScheduler.h"

pthread_mutex_t shortTermSchedulerMutex;
bool enableShortTermScheduler;

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
	enableShortTermScheduler = true;
}

void destroyshortTermScheduler() {
	pthread_mutex_destroy(&shortTermSchedulerMutex);
	sem_destroy(&processInReady_sem);
}

void shortTermScheduler_lock() {
	if (enableShortTermScheduler) {
		enableShortTermScheduler = false;
		logInfo("El planificador de corto plazo fue bloqueado");
	} else {
		logInfo("El planificador de corto plazo ya se encontraba bloqueado");
	}
}

void shortTermScheduler_unlock() {
	if (!enableShortTermScheduler) {
		shortTermSchedulerMutex_unlock();
		enableShortTermScheduler = true;
		logInfo("El planificador de corto plazo fue desbloqueado");
	} else {
		logInfo("El planificador de corto plazo ya se encontraba desbloqueado");
	}
}

void shortTermSchedulerMutex_lock() {
	pthread_mutex_lock(&shortTermSchedulerMutex);
}

void shortTermSchedulerMutex_unlock() {
	pthread_mutex_unlock(&shortTermSchedulerMutex);
}

bool isEnableShortTermScheduler() {
	return enableShortTermScheduler;
}

void processInReady_wait() {
	sem_wait(&processInReady_sem);
	logInfo("Se decremento (wait) el recurso de 'Procesos en ready'");
}

void processInReady_signal() {
	logInfo("Se incremento (signal) el recurso de 'Procesos en ready'");
	sem_post(&processInReady_sem);
}
