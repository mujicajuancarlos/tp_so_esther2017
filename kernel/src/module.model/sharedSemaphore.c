/*
 * sharedSemaphore.c
 *
 *  Created on: 6/6/2017
 *      Author: utnso
 */

#include "../module.planning/processLifeCycle.h"//dejar en este arvhico porque tiene dependencias
#include "sharedSemaphore.h"

t_dictionary* semaphores;

pthread_mutex_t semaphoresMutex;

void initializeSemaphores(char** semKeys, char** semValues) {

	pthread_mutex_init(&semaphoresMutex, NULL);

	semaphores = dictionary_create();
	int index = 0;
	while (semKeys[index] != NULL) {
		int* value = malloc(sizeof(int));
		*value = atoi(semValues[index]);
		dictionary_put(semaphores, semKeys[index], value);
		index++;
	}
	initializeQueueBySemaphore(semKeys);
}

int executeBasicWait(t_nombre_semaforo semKey, bool* shouldLock) {
	int* value;
	int status;
	pthread_mutex_lock(&semaphoresMutex);
	if (dictionary_has_key(semaphores, semKey)) {
		value = dictionary_get(semaphores, semKey);
		*value = *value - 1;
		*shouldLock = *value < 0;
		logInfo("El semaforo: %s ahora tiene el valor %d", semKey, *value);
		status = UPDATE_SEM_SUCCESS;
	} else {
		logError("Se intento ejecutar wait sobre el semaforo: %s que no existe",
				semKey);
		shouldLock = NULL;
		status = UPDATE_SEM_FAILURE;
	}
	pthread_mutex_unlock(&semaphoresMutex);
	return status;
}

int executeBasicSignal(t_nombre_semaforo semKey, bool* shouldUnlock) {
	int* value;
	int status;
	pthread_mutex_lock(&semaphoresMutex);
	if (dictionary_has_key(semaphores, semKey)) {
		value = dictionary_get(semaphores, semKey);
		*value = *value + 1;
		*shouldUnlock = *value <= 0;
		logInfo("El semaforo: %s ahora tiene el valor %d", semKey, *value);
		status = UPDATE_SEM_SUCCESS;
	} else {
		logError(
				"Se intento ejecutar signal sobre el semaforo: %s que no existe",
				semKey);
		shouldUnlock = NULL;
		status = UPDATE_SEM_FAILURE;
	}
	pthread_mutex_unlock(&semaphoresMutex);
	return status;
}
