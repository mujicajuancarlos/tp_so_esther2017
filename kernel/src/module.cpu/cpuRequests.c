/*
 * cpuRequests.c
 *
 *  Created on: 23/5/2017
 *      Author: utnso
 */

#include "cpuRequest.h"

/*
 * soy invocado por el planificador para ejecutar un proceso,
 * puede ser nuevo o un proceso que fue bloqueado y esta listo para ejecutar nuevamente
 */
void startProcessExecution(Process* selectedProcess, CPU* selectedCPU) {
	sendToEXEC(selectedProcess);
	selectedCPU->process = selectedProcess;
	logInfo("Iniciando ejecucion de pid: %d en cpu: %d", selectedProcess->pid,
			selectedCPU->fileDescriptor);
	char* buffer = serialize_PCB(selectedProcess->pcb);
	uint32_t size = sizeOf_PCB(selectedProcess->pcb);
	Package* package = createAndSendPackage(selectedCPU->fileDescriptor,
	COD_EXEC_NEW_PCB, size, buffer);
	if (package == NULL) {
		logError("La CPU no pudo recibir la solicitud de ejecutar el proceso.");
		moveFromExcecToReady(selectedProcess);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		removeCPU(selectedCPU);
	}
	free(buffer);
	destroyPackage(package);
}

void continueCurrentExcecution(CPU* cpu) {
	logInfo("Continuando ejecucion de pid: %d en cpu: %d", cpu->process->pid,
			cpu->fileDescriptor);
	Package* package = createAndSendPackage(cpu->fileDescriptor,
	COD_CONTINUE_EXECUTION, 0, NULL);
	if (package == NULL) {
		logError("La CPU no pudo recibir la solicitud de ejecutar el proceso.");
		moveFromExcecToReady(cpu->process);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		removeCPU(cpu);
	}
	destroyPackage(package);
}

void contextSwitch(CPU* cpu) {
	logInfo(
			"Realizando un context switch solicito a la cpu el pcb actualizado de pid: %d en cpu: %d",
			cpu->process->pid, cpu->fileDescriptor);
	Package* package = createAndSendPackage(cpu->fileDescriptor,
	COD_CONTEXT_SWITCH_REQUEST, 0, NULL);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpu->fileDescriptor);
		if (package != NULL && package->msgCode == COD_CONTEXT_SWITCH_RESPONSE) {
			PCB* newPcb = deserialize_PCB(package->stream);
			replacePCB(cpu->process, newPcb);
			moveFromExcecToReady(cpu->process);
			markFreeCPU(cpu);
		} else {
			logError("La CPU no envio respondio el context switch");
			moveFromExcecToReady(cpu->process);
			logInfo("Finalizando la CPU que no acepta solicitudes");
			removeCPU(cpu);
		}
		destroyPackage(package);
	} else {
		logError("La CPU no pudo recibir la solicitud de ejecutar el proceso.");
		moveFromExcecToReady(cpu->process);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		removeCPU(cpu);
	}
}

void contextSwitchForBlocked(CPU* cpu, t_nombre_semaforo semId) {
	logInfo("Esperando el pcb actualizado para el proceso %d desde la cpu: %d",
			cpu->process->pid, cpu->fileDescriptor);
	Package* package = createAndReceivePackage(cpu->fileDescriptor);
	if (package != NULL) {
		if (package->msgCode == COD_CONTEXT_SWITCH_RESPONSE) {
			PCB* newPcb = deserialize_PCB(package->stream);
			replacePCB(cpu->process, newPcb);
			notifyLockedProcessFor(semId, cpu->process);
			markFreeCPU(cpu);
		} else {
			logError("La CPU no devolvio el pcb actualizado");
			moveFromExcecToReady(cpu->process);
			logInfo(
					"Remuevo la cpu porque no esta respondiendo adecuadamente.");
			removeCPU(cpu);
		}
		destroyPackage(package);
	} else {
		logError("La CPU no devolvio el pcb actualizado");
		moveFromExcecToReady(cpu->process);
		logInfo("Remuevo la cpu porque no esta respondiendo adecuadamente.");
		removeCPU(cpu);
	}

}

void contextSwitchForForceQuitProcess(CPU* cpu) {
	logInfo("Solicito a la cpu el pcb actualizado de pid: %d en cpu: %d",
			cpu->process->pid, cpu->fileDescriptor);
	Package* package = createAndSendPackage(cpu->fileDescriptor,
	COD_CONTEXT_SWITCH_REQUEST, 0, NULL);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpu->fileDescriptor);
		if (package != NULL && package->msgCode == COD_CONTEXT_SWITCH_RESPONSE) {
			PCB* newPcb = deserialize_PCB(package->stream);
			replacePCB(cpu->process, newPcb);
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_END_PROCESS_BY_REQUEST);
			markFreeCPU(cpu);
		} else {
			logError(
					"La CPU no devolvio el pcb actualizado, de todas formas se finalizara el proceso");
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_END_PROCESS_BY_REQUEST);
			logInfo("Finalizando la CPU que no acepta solicitudes");
			removeCPU(cpu);
		}
		destroyPackage(package);
	} else {
		logError(
				"La CPU no devolvio el pcb actualizado, de todas formas se finalizara el proceso");
		moveFromExcecToExit_withError(cpu->process,
		SC_ERROR_END_PROCESS_BY_REQUEST);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		removeCPU(cpu);
	}
}

void executeWaitTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = package->stream;
	bool shouldLock = false;
	bool hasError = executeBasicWait(semId, &shouldLock) == UPDATE_SEM_SUCCESS;
	notifyUpdateSemaphoreStatus(cpu, hasError, shouldLock);
	if (!hasError) {
		if (shouldLock) {
			logInfo(
					"El proceso pid: %d quedara bloqueado despues del wait en %s",
					cpu->process->pid, semId);
			contextSwitchForBlocked(cpu, semId); //recibo el pcb para actualizarlo y enviar el proceso a bloqueado
		} else {
			logInfo(
					"El proceso pid: %d NO quedara bloqueado despues del wait en %s",
					cpu->process->pid, semId);
		}
	} else {
		moveFromExcecToExit_withError(cpu->process, SC_ERROR_WAIT_SEMAPHORE);
		markFreeCPU(cpu);
	}
}

void executeSignalTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = package->stream;
	bool shouldUnlock = false;
	bool hasError = executeBasicSignal(semId,
			&shouldUnlock) == UPDATE_SEM_SUCCESS;
	notifyUpdateSemaphoreStatus(cpu, hasError, false); //notifico el estado, siempre enviar false
	if (!hasError) {
		if (shouldUnlock) {
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
		moveFromExcecToExit_withError(cpu->process, SC_ERROR_WAIT_SEMAPHORE);
		markFreeCPU(cpu);
	}
}

void executeSetSharedVar(CPU* cpu, Package* package) {
	set_shared_var* data = deserialize_SetSharedVar(package->stream);
	Package* tmpPackage;
	if (setSharedVar(data->name, data->newValue) == UPDATE_VAR_SUCCESS) {
		char* buffer = serialize_int(data->newValue);
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
						COD_SYSCALL_SUCCESS, sizeof(int), buffer);
		free(buffer);
	} else {
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process, SC_ERROR_SET_SHARED_VAR);
		markFreeCPU(cpu);
	}
	destroySetSharedVar(data);
	destroyPackage(tmpPackage);
}

void executeGetSharedVar(CPU* cpu, Package* package) {
	char* key = package->stream;
	int value;
	Package* tmpPackage;
	if (getSharedVar(key, &value) == UPDATE_VAR_SUCCESS) {
		char* buffer = serialize_int(value);
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_SYSCALL_SUCCESS, sizeof(int), buffer);
		free(buffer);
	} else {
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process, SC_ERROR_GET_SHARED_VAR);
		markFreeCPU(cpu);
	}
	destroyPackage(tmpPackage);
}

void resolveRequest_endInstruction(CPU* cpu, Package* package) {
	if (!cpu->process->forceQuit) {
		int algorithm = getAlgorithmIndex(cpu->kernelStruct->config->algoritmo);
		switch (algorithm) {
		case ROUND_ROBIN:
			cpu->process->quantum--;
			if (cpu->process->quantum > 0) {
				continueCurrentExcecution(cpu);
			} else {
				contextSwitch(cpu);
			}
			break;
		case FIFO:
		default:
			continueCurrentExcecution(cpu);
			break;
		}
	} else {
		contextSwitchForForceQuitProcess(cpu);
	}
}

void resolveRequest_programFinished(CPU* cpu, Package* package) {
	PCB* newPcb = deserialize_PCB(package->stream);
	replacePCB(cpu->process, newPcb);
	moveFromExcecToExit_withoutError(cpu->process);
	markFreeCPU(cpu);
}

void resolveRequest_cpuDisconnected(CPU* cpu, Package* package) {
	PCB* newPcb = deserialize_PCB(package->stream);
	replacePCB(cpu->process, newPcb);
	moveFromExcecToReady(cpu->process);
	removeCPU(cpu);
}

void resolveRequest_sharedVarOperation(CPU* cpu, Package* package) {
	switch (package->msgCode) {
	case COD_SET_SHARED_VAR:
		executeSetSharedVar(cpu, package);
		break;
	case COD_GET_SHARED_VAR:
		executeGetSharedVar(cpu, package);
		break;
	}
}

void resolveRequest_dynamicMemoryOperation(CPU* cpu, Package* package) {

}

void resolveRequest_fileSystemOperation(CPU* cpu, Package* package) {

}

void resolveRequest_updateSemaphore(CPU* cpu, Package* package) {
	switch (package->msgCode) {
	case COD_SEM_WAIT:
		executeWaitTo(cpu, package);
		break;
	case COD_SEM_SIGNAL:
		executeSignalTo(cpu, package);
		break;
	}
}

void resolveRequest_executionError(CPU* cpu, Package* package) {
	//todo enviar el proceso a exit con el exit code
	//la cpu queda limpia
}
