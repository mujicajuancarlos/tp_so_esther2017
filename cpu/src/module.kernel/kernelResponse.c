/*
 * kernelResponse.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "kernelResponse.h"

void reportEndInstruction(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel que la instruccion se ejecutó correctamente");
	sendPCB(cpuStruct, COD_END_INSTRUCCION);
}

void reportEndProcess(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel que el programa finalizo con éxito");
	sendPCB(cpuStruct, COD_PROGRAM_FINISHED);
	unloadPCB(cpuStruct);
}

void reportContextSwich(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel en nuevo pcb producto del contex swich");
	sendPCB(cpuStruct, COD_CONTEXT_SWITCH_RESPONSE);
	unloadPCB(cpuStruct);
}

void sendPCB(cpu_struct* cpuStruct, int messageCode) {
	PCB* newPCB = getPCB();
	char* buffer = serialize_PCB(newPCB);
	uint32_t size = sizeOf_PCB(newPCB);
	Package* package = createAndSendPackage(cpuStruct->socketClientKernel,
			messageCode, size, buffer);
	logTrace("Se envio en PCB del pid: %d actualizado al kernel fd:%d",
			newPCB->pid, cpuStruct->socketClientKernel);
	destroyPackage(package);
}

void reportExcecutionError(cpu_struct* cpuStruct, int errorFlag) {
	Package* package;
	switch (errorFlag) {
	case FLAG_STACKOVERFLOW:
	case FLAG_SEGMENTATION_FAULT:
	case FLAG_DISCONNECTED_MEMORY:
		package = createAndSendPackage(cpuStruct->socketClientKernel,
				SC_ERROR_MEMORY_EXCEPTION, 0, NULL);
		break;
	case FLAG_UNKNOWN_ERROR:
		package = createAndSendPackage(cpuStruct->socketClientKernel,
				SC_ERROR_UNKNOWN, 0, NULL);
		break;
	default:
		package = createAndSendPackage(cpuStruct->socketClientKernel,
				SC_ERROR_UNKNOWN, 0, NULL);
		break;
	}
	destroyPackage(package);
}
