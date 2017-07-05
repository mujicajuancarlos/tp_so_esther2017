/*
 * multiprogramming.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#include "longTermScheduler.h"

sem_t multiprogrammingLevelSem;

void initializeMultiprogrammingLevel(kernel_struct* kernel_struct) {
	sem_init(&multiprogrammingLevelSem, 1,
			kernel_struct->config->grado_multiprog);
}

void destroyMultiprogrammingLevel() {
	sem_destroy(&multiprogrammingLevelSem);
}

void updateMultiprogrammingLevel(kernel_struct* kernel_struct, int newLevel) {
	int currentLevel = kernel_struct->config->grado_multiprog;
	if ((newLevel >= 0) && (newLevel <= MAX_MULTIPROGRAMMING_LVL)) {
		logInfo("Actualizando el grado de multiprogramación a %d", newLevel);
		if (newLevel > currentLevel)
			incrementMultiprogrammingLevel(newLevel - currentLevel);
		if (newLevel < currentLevel) {
			pthread_t thread;
			pthread_attr_t threadAttr;
			int* decrementNumber = malloc(sizeof(int));
			*decrementNumber = currentLevel - newLevel;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			pthread_create(&thread, &threadAttr, (void*) decrementMultiprogrammingLevel,
					decrementNumber);
		}
		kernel_struct->config->grado_multiprog = newLevel;
	} else
		logError("No es posible actualizar el grado de multiprogramacion a %d",
				newLevel);
}

void incrementMultiprogrammingLevel(int number) {
	int i;
	for (i = 0; i < number; i++) {
		_incrementMultiprogrammingLevel();
	}
}

/**
 * me ejecuto en un hilo porque la operacion wait es bloqueante
 */
void decrementMultiprogrammingLevel(int* number) {
	int i;
	for (i = 0; i < *number; i++) {
		_decrementMultiprogrammingLevel();
		logInfo("Se decremento en 1 el nivel de multiprogramacion");
	}
	free(number);
}

/*
 * realizo el signal del semaforo + logueo
 */
void _incrementMultiprogrammingLevel(){
	sem_post(&multiprogrammingLevelSem);
	logInfo("Se incremento en 1 el nivel de multiprogramacion");
}

/*
 * realizo wait del semaforo + logueo
 */
void _decrementMultiprogrammingLevel(){
	sem_wait(&multiprogrammingLevelSem);
}
