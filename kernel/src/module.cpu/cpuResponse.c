/*
 * cpuResponse.c
 *
 *  Created on: 31/5/2017
 *      Author: utnso
 */

#include "cpuResponse.h"

void executeWaitTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = package->stream;
	bool shouldBlock = executeBasicWait(semId);
	if (shouldBlock) {
		contextSwitchForBlocked(cpu, semId);
	} else {
		logInfo(
				"El proceso pid: %d NO quedara bloqueado despues del wait en %s",
				cpu->process->pid, semId);
	}
}

void executeSignalTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = package->stream;
	executeBasicSignal(semId);
	logInfo("El proceso de pid: %d ejecuto signal sobre el semaforo %s",
			cpu->process->pid, semId);
}
