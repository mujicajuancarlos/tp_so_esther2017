/*
 * cpuResponse.c
 *
 *  Created on: 31/5/2017
 *      Author: utnso
 */

#include "cpuResponse.h"

void executeWaitTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = package->stream;
	bool* shouldLock = malloc(sizeof(bool));
	if (executeBasicWait(semId, shouldLock) == UPDATE_SEM_SUCCESS) {
		if (*shouldLock) {
			logInfo(
					"El proceso pid: %d quedara bloqueado despues del wait en %s",
					cpu->process->pid, semId);
			contextSwitchForBlocked(cpu, semId);
		} else {
			logInfo(
					"El proceso pid: %d NO quedara bloqueado despues del wait en %s",
					cpu->process->pid, semId);
		}
	} else {
		logError("No se pudo ejecutar wait");
		//TODO: notificar el error a cpu para ejecutar la rutina de error de ejecucion o finalizar automaticamente
	}
	free(shouldLock);
}

void executeSignalTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = package->stream;
	bool* shouldUnock = malloc(sizeof(bool));
	if (executeBasicSignal(semId, shouldUnock) == UPDATE_SEM_SUCCESS) {
		if (*shouldUnock) {
			logInfo(
					"Se desbloqueara un proceso de la cola de bloqueados del semaforo: %s",
					semId);
			notifyUnlockedProcessFor(semId);
		} else {
			logInfo(
					"No se desbloqueara ningun proceso de la cola de bloqueados del semaforo: %s",
					semId);
		}
	} else {
		logError("No se pudo ejecutar wait");
		//TODO: notificar el error a cpu para ejecutar la rutina de error de ejecucion o finalizar automaticamente
	}
	free(shouldUnock);
}
