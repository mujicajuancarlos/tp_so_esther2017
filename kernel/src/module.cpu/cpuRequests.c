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
void startProcessExecution(Process* selectedProcess, CPU* selectedCPU, int cpuFD) {
	sendToEXEC(selectedProcess);
	logInfo("Iniciando ejecucion de pid: %d en cpu: %d", selectedProcess->pid,
			selectedCPU->fileDescriptor);
	char* buffer = serialize_PCB(selectedProcess->pcb);
	uint32_t size = sizeOf_PCB(selectedProcess->pcb);
	selectedCPU->process = selectedProcess;
	Package* package = createAndSendPackage(cpuFD,
	COD_EXEC_NEW_PCB, size, buffer);
	if (package == NULL) {
		selectedCPU->process = NULL;
		logError("La CPU no pudo recibir la solicitud de ejecutar el proceso.");
		moveFromExcecToReady(selectedProcess);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		//removeCPU(selectedCPU);
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
	t_nombre_semaforo semId = string_substring_until(package->stream,
			package->size);
	semaphoresRequestMutex_lock();
	bool shouldLock = false;
	bool hasError = executeBasicWait(semId, &shouldLock) != UPDATE_SEM_SUCCESS;
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
	free(semId);
	semaphoresRequestMutex_unlock();
}

void executeSignalTo(CPU* cpu, Package* package) {
	t_nombre_semaforo semId = string_substring_until(package->stream,
			package->size);
	semaphoresRequestMutex_lock();
	bool shouldUnlock = false;
	bool hasError = executeBasicSignal(semId,
			&shouldUnlock) != UPDATE_SEM_SUCCESS;
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
	free(semId);
	semaphoresRequestMutex_unlock();
}

void executeSetSharedVar(CPU* cpu, Package* package) {
	set_shared_var* data = deserialize_SetSharedVar(package->stream);
	char* key = string_new();
	if (!string_starts_with(data->name, SHARED_VAR_PREFIX)) {
		string_append(&key, SHARED_VAR_PREFIX);
	}
	string_append(&key, data->name);
	Package* tmpPackage;
	if (setSharedVar(key, data->newValue) == UPDATE_VAR_SUCCESS) {
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
	free(key);
	destroySetSharedVar(data);
	destroyPackage(tmpPackage);
}

void executeGetSharedVar(CPU* cpu, Package* package) {
	char* key = string_new();
	char* varName = string_substring_until(package->stream, package->size);
	if (!string_starts_with(varName, SHARED_VAR_PREFIX)) {
		string_append(&key, SHARED_VAR_PREFIX);
	}
	string_append(&key, varName);
	free(varName);
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
	free(key);
	destroyPackage(tmpPackage);
}

void executeMallocMemoryTo(CPU* cpu, Package* package) {
	t_puntero pointer;
	char* buffer;
	Package* tmpPackage = NULL;
	int mallocSize = deserialize_int(package->stream);
	int status = basicMallocMemory(cpu->process, mallocSize, &pointer);
	switch (status) {
	case MALLOC_MEMORY_SUCCES:
		buffer = serialize_int(pointer);
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, sizeof(t_puntero), buffer);
		free(buffer);
		break;
	case SC_ERROR_MEMORY_EXCEPTION:
	case SC_ERROR_MEMORY_ALLOC_EXCEEDED:
	case SC_ERROR_ADD_PAGE_REFUSED:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process, status);
		markFreeCPU(cpu);
		break;
	}
	destroyPackage(tmpPackage);
}

void executeFreeMemoryTo(CPU* cpu, Package* package) {
	t_puntero pointer = deserialize_int(package->stream);
	Package* tmpPackage = NULL;
	int status = basicFreeMemory(cpu->process, pointer);
	switch (status) {
	case FREE_MEMORY_SUCCES:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, 0, NULL);
		break;
	case SC_ERROR_MEMORY_EXCEPTION:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process, status);
		markFreeCPU(cpu);
		break;
	}
	destroyPackage(tmpPackage);
}

void executeOpenProcessFileTo(CPU* cpu, Package* package) {
	t_new_FD_request* request = deserialize_t_new_FD_request(package->stream);
	Package* tmpPackage = NULL;
	char* buffer;
	int assignedFD;
	int status = basicOpenProcessFile(cpu->process, request, &assignedFD);
	destroy_t_new_FD_request(request);
	switch (status) {
	case OPEN_FD_SUCCESS:
		buffer = serialize_int(assignedFD);
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, sizeof(uint32_t), buffer);
		free(buffer);
		break;
	case PERMISSIONS_DENIED_FD_FAILURE:
	case WITHOUT_RESOURCES_FD_FAILURE:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process,
		SC_ERROR_FILE_CREATE_REFUSED);
		markFreeCPU(cpu);
		break;
	}
	destroyPackage(tmpPackage);
}

void executeDeleteProcessFileTo(CPU* cpu, Package* package) {
	Package* tmpPackage = NULL;
	int fileDescriptor = deserialize_int(package->stream);
	int status = basicDeleteProcessFile(cpu->process, fileDescriptor);
	switch (status) {
	case DELETE_FD_SUCCESS:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, 0, NULL);
		break;
	case FILE_NOTFOUND_FD_FAILURE:
	case FILE_IN_USED_FD_FAILUERE:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		switch (status) {
		case FILE_NOTFOUND_FD_FAILURE:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_FILE_NOT_FOUND);
			break;
		case FILE_IN_USED_FD_FAILUERE:
			moveFromExcecToExit_withError(cpu->process, SC_ERROR_FILE_IN_USE);
			break;
		default:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_UNKNOWN);
			break;
		}
		break;
		markFreeCPU(cpu);
	}
	destroyPackage(tmpPackage);
}

void executeCloseProcessFileTo(CPU* cpu, Package* package) {
	Package* tmpPackage = NULL;
	int fileDescriptor = deserialize_int(package->stream);
	int status = basicCloseProcessFile(cpu->process, fileDescriptor);
	switch (status) {
	case CLOSE_FD_SUCCESS:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, 0, NULL);
		break;
	case FILE_NOTFOUND_FD_FAILURE:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process,
		SC_ERROR_FILE_NOT_FOUND);
		markFreeCPU(cpu);
		break;
	}
	destroyPackage(tmpPackage);
}

void executeSeekProcessFileTo(CPU* cpu, Package* package) {
	t_seed_FD_request* request = deserialize_t_seed_FD_request(package->stream);
	Package* tmpPackage = NULL;
	int status = basicSeekProcessFile(cpu->process, request);
	destroy_t_seed_FD_request(request);
	switch (status) {
	case SEEK_FD_SUCCESS:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, 0, NULL);
		break;
	case FILE_NOTFOUND_FD_FAILURE:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		moveFromExcecToExit_withError(cpu->process,
		SC_ERROR_FILE_NOT_FOUND);
		markFreeCPU(cpu);
		break;
	}
	destroyPackage(tmpPackage);
}

void executeWriteProcessFileTo(CPU* cpu, Package* package) {
	t_data_FD_request* request = deserialize_t_data_FD_request(package->stream);
	Package* tmpPackage = NULL;
	int status = basicWriteProcessFile(cpu->process, request);
	destroy_t_data_FD_request(request);
	switch (status) {
	case WRITE_FD_SUCCESS:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, 0, NULL);
		break;
	case FILE_NOTFOUND_FD_FAILURE:
	case PERMISSIONS_DENIED_FD_FAILURE:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		switch (status) {
		case FILE_NOTFOUND_FD_FAILURE:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_FILE_NOT_FOUND);
			break;
		case PERMISSIONS_DENIED_FD_FAILURE:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_FILE_WRITE_REFUSED);
			break;
		default:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_UNKNOWN);
			break;
		}
		break;
		markFreeCPU(cpu);
	}
	destroyPackage(tmpPackage);
}

void executeReadProcessFileTo(CPU* cpu, Package* package) {
	t_dataPointer_FD_request* request = deserialize_t_dataPointer_FD_request(
			package->stream);
	Package* tmpPackage = NULL;
	int status = basicReadProcessFile(cpu->process, request);
	free(request);
	switch (status) {
	case READ_FD_SUCCESS:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_SUCCESS, 0, NULL);
		break;
	case FILE_NOTFOUND_FD_FAILURE:
	case MEMORY_SAVE_FAILURE:
	case PERMISSIONS_DENIED_FD_FAILURE:
	case FS_FILE_BLOCK_FAULT:
	default:
		tmpPackage = createAndSendPackage(cpu->fileDescriptor,
		COD_SYSCALL_FAILURE, 0, NULL);
		switch (status) {
		case FILE_NOTFOUND_FD_FAILURE:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_FILE_NOT_FOUND);
			break;
		case PERMISSIONS_DENIED_FD_FAILURE:
		case FS_FILE_BLOCK_FAULT:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_FILE_READ_REFUSED);
			break;
		case MEMORY_SAVE_FAILURE:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_MEMORY_EXCEPTION);
			break;
		default:
			moveFromExcecToExit_withError(cpu->process,
			SC_ERROR_UNKNOWN);
			break;
		}
		break;
		markFreeCPU(cpu);
	}
	destroyPackage(tmpPackage);
}

/*
 * MANEJAN LAS SOLUCITUDES DEL CPU
 */
void resolveRequest_endInstruction(CPU* cpu, Package* package) {
	logTrace("La cpu %d informo que ejecuto la instruccion ansisop",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->burst_Counter), 1);
	if (!cpu->process->forceQuit) {
		int algorithm = getAlgorithmIndex(cpu->kernelStruct->config->algoritmo);
		switch (algorithm) {
		case ROUND_ROBIN:
			cpu->process->quantum--;
			if (cpu->process->quantum > 0) {
				timeoutForInstruction(cpu);
				continueCurrentExcecution(cpu);
			} else {
				contextSwitch(cpu);
			}
			break;
		case FIFO:
		default:
			timeoutForInstruction(cpu);
			continueCurrentExcecution(cpu);
			break;
		}
	} else {
		contextSwitchForForceQuitProcess(cpu);
	}
}

void resolveRequest_programFinished(CPU* cpu, Package* package) {
	logTrace("La cpu %d informo que el programa ansisop finalizó",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->burst_Counter), 1);
	PCB* newPcb = deserialize_PCB(package->stream);
	replacePCB(cpu->process, newPcb);
	moveFromExcecToExit_withoutError(cpu->process);
	markFreeCPU(cpu);
}

void resolveRequest_cpuDisconnected(CPU* cpu, Package* package) {
	logTrace(
			"La cpu %d informo que se va desconectar pero termino la ejecucion de la instruccion ansisop",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->burst_Counter), 1);
	PCB* newPcb = deserialize_PCB(package->stream);
	replacePCB(cpu->process, newPcb);
	moveFromExcecToReady(cpu->process);
	removeCPU(cpu);
}

void resolveRequest_sharedVarOperation(CPU* cpu, Package* package) {
	logTrace(
			"La cpu %d solicito la ejecucion de syscall de variables compartidas",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->sysC_Counter), 1);
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
	logTrace("La cpu %d solicito la ejecucion de syscall de memoria dinamica",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->sysC_Counter), 1);
	switch (package->msgCode) {
	case COD_MALLOC_MEMORY:
		executeMallocMemoryTo(cpu, package);
		break;
	case COD_FREE_MEMORY:
		executeFreeMemoryTo(cpu, package);
		break;
	}
}

void resolveRequest_fileSystemOperation(CPU* cpu, Package* package) {
	logTrace("La cpu %d solicito la ejecucion de syscall de file system",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->sysC_Counter), 1);
	switch (package->msgCode) {
	case COD_OPEN_FD:
		executeOpenProcessFileTo(cpu, package);
		break;
	case COD_DELETE_FD:
		executeDeleteProcessFileTo(cpu, package);
		break;
	case COD_CLOSE_FD:
		executeCloseProcessFileTo(cpu, package);
		break;
	case COD_SEEK_FD:
		executeSeekProcessFileTo(cpu, package);
		break;
	case COD_WRITE_FD:
		executeWriteProcessFileTo(cpu, package);
		break;
	case COD_READ_FD:
		executeReadProcessFileTo(cpu, package);
		break;
	}
}

void resolveRequest_updateSemaphore(CPU* cpu, Package* package) {
	logTrace("La cpu %d solicito la ejecucion de syscall semaforos",
			cpu->fileDescriptor);
	incrementCounter(&(cpu->process->processCounters->sysC_Counter), 1);
	switch (package->msgCode) {
	case COD_SEM_WAIT:
		executeWaitTo(cpu, package);
		break;
	case COD_SEM_SIGNAL:
		executeSignalTo(cpu, package);
		break;
	}
}

/*
 * cuando la cpu lanza un error invoca al kernel esta funcion para que el proceso finalize y libere la cpu
 */
void resolveRequest_executionError(CPU* cpu, Package* package) {
	logTrace(
			"La cpu %d informo un error en la ejecucion de la instruccion ansisop",
			cpu->fileDescriptor);
	uint32_t exitCode = deserialize_int(package->stream);
	moveFromExcecToExit_withError(cpu->process, exitCode);
	markFreeCPU(cpu);
}

void timeoutForInstruction(CPU* cpu) {
	int delay;
	logTrace("El quantum_sleep esta realizando un retardo de: %d  ",
			cpu->kernelStruct->config->quantum_sleep);
	//Paso de milisegundos a microsegundos
	delay = cpu->kernelStruct->config->quantum_sleep * 1000 ;
	usleep(delay);
	logTrace("Finaliza retardo");
}

