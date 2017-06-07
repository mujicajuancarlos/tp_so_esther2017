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
			"Voy a realizar un context switch solicito a la cpu el pcb actualizado de pid: %d en cpu: %d",
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
			destroyPackage(package);
		} else {
			logError("La CPU no envio respondio el context switch");
			moveFromExcecToReady(cpu->process);
			logInfo("Finalizando la CPU que no acepta solicitudes");
			removeCPU(cpu);
		}
	} else {
		logError("La CPU no pudo recibir la solicitud de ejecutar el proceso.");
		moveFromExcecToReady(cpu->process);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		removeCPU(cpu);
	}
}

void contextSwitchForBlocked(CPU* cpu, t_nombre_semaforo semId) {
	logInfo(
			"Voy a realizar un context switch solicito a la cpu el pcb actualizado de pid: %d en cpu: %d",
			cpu->process->pid, cpu->fileDescriptor);
	Package* package = createAndSendPackage(cpu->fileDescriptor,
	COD_CONTEXT_SWITCH_REQUEST, 0, NULL);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpu->fileDescriptor);
		if (package != NULL && package->msgCode == COD_CONTEXT_SWITCH_RESPONSE) {
			PCB* newPcb = deserialize_PCB(package->stream);
			replacePCB(cpu->process, newPcb);
			notifyLockedProcessFor(semId,cpu->process);
			markFreeCPU(cpu);
			destroyPackage(package);
		} else {
			logError("La CPU no envio respondio el context switch");
			moveFromExcecToReady(cpu->process);
			logInfo("Finalizando la CPU que no acepta solicitudes");
			removeCPU(cpu);
		}
	} else {
		logError("La CPU no pudo recibir la solicitud de ejecutar el proceso.");
		moveFromExcecToReady(cpu->process);
		logInfo("Finalizando la CPU que no acepta solicitudes");
		removeCPU(cpu);
	}
}

void programFinished(CPU* cpu, Package* package) {
	PCB* newPcb = deserialize_PCB(package->stream);
	replacePCB(cpu->process, newPcb);
	moveFromExcecToExit(cpu->process);
	markFreeCPU(cpu);
}

void cpuDisconnected(CPU* cpu, Package* package) {
	PCB* newPcb = deserialize_PCB(package->stream);
	replacePCB(cpu->process, newPcb);
	moveFromExcecToReady(cpu->process);
	removeCPU(cpu);
}
