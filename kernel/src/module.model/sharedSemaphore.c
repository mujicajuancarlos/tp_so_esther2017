/*
 * sharedSemaphore.c
 *
 *  Created on: 6/6/2017
 *      Author: utnso
 */

#include "../module.planning/processLifeCycle.h"//dejar en este arvhico porque tiene dependencias
#include "sharedSemaphore.h"

t_dictionary* semaphores;

void initializeSemaphores(char** semKeys, char** semValues) {
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

bool executeBasicWait(t_nombre_semaforo semKey) {
	int* value;
	if (dictionary_has_key(semaphores, semKey)) {
		value = dictionary_get(semaphores, semKey);
		*value = *value - 1;
	} else {
		logError("Se intento ejecutar wait sobre el semaforo: %s que no existe",
				semKey);
		exit(-1);
	}
	logInfo("El semaforo: %s ahora tiene el valor %d", semKey, value);
	if(*value < 0){
		logTrace("Bloquando el proceso que invoco a wait de semaforo: %s",semKey);
	}
	return *value < 0;
}

void executeBasicSignal(t_nombre_semaforo semKey) {
	int* value;
	if (dictionary_has_key(semaphores, semKey)) {
		value = dictionary_get(semaphores, semKey);
		*value = *value + 1;
	} else {
		logError(
				"Se intento ejecutar signal sobre el semaforo: %s que no existe",
				semKey);
		exit(-1);
	}
	logInfo("El semaforo: %s ahora tiene el valor %d", semKey, *value);
	if(*value <= 0){
		logTrace("Desbloqueando algun proceso bloqueado por el semaforo: %s",semKey);
		notifyUnlockedProcessFor(semKey);
	}
}
