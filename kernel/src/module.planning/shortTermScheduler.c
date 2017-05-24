/*
 * shortTermScheduler.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#include "shortTermScheduler.h"

pthread_mutex_t shortTermSchedulerMutex;

void initializeshortTermScheduler() {
	pthread_mutex_init(&shortTermSchedulerMutex, NULL);
}

void destroyshortTermScheduler() {
	pthread_mutex_destroy(&shortTermSchedulerMutex);
}

void shortTermScheduler_lock() {
	pthread_mutex_lock(&readyListMutex);
	logInfo("Planificador de corto plazo bloqueado");
}

void shortTermScheduler_unlock() {
	pthread_mutex_unlock(&readyListMutex);
	logInfo("Planificador de corto plazo desbloqueado");
}
