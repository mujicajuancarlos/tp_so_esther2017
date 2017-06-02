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
	case FLAG_END_PROGRAM:
		reportEndProcess(cpuStruct);
		break;
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
	case FLAG_DISCONNECTED_KERNEL:
		logError("El kernel esta desconectado, no puedo continuar ejecuarnado");
		logTrace(
				"Finalizando proceso cpu porque no se pudo conectar con el kernel");
		exit(1);
		break;
	case FLAG_UNKNOWN_MESSAGE_CODE:
		logError(
				"El kernel no esta respondiendo adecuadamente, no puedo continuar ejecuarnado");
		logTrace("Finalizando proceso cpu porque hay errores en el kernel");
		exit(1);
		break;
	default:
		package = createAndSendPackage(cpuStruct->socketClientKernel,
		SC_ERROR_UNKNOWN, 0, NULL);
		break;
	}
	destroyPackage(package);
}

int getSharedVarriableValue(cpu_struct* cpuStruct, char* name) {
	Package* package;
	package = createAndSendPackage(cpuStruct->socketClientKernel,
	COD_GET_SHARED_VAR, strlen(name), name);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpuStruct->socketClientKernel);
		if (package != NULL) {
			int value;
			switch (package->msgCode) {
			case COD_GET_SHARED_VAR_RESPONSE:
				value = deserialize_int(package->stream);
				destroyPackage(package);
				return value;
				break;
			default:
				logError(
						"Kernel respondio con un codigo de mensaje desconocido");
				setErrorFlag(FLAG_UNKNOWN_MESSAGE_CODE);
				break;
			}
		} else {
			logError("No se pudo recibir los datos del kernel");
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	} else {
		logError("No se pudo enviar la solicitud al kernel");
		setErrorFlag(FLAG_DISCONNECTED_KERNEL);
	}
	return NULL_VALUE;
}

int setSharedVarriableValue(cpu_struct* cpuStruct, char* name, int value) {
	Package* package;
	set_shared_var* object = createSetSharedVar(name, value);
	char* buffer = serialize_SetSharedVar(object);
	package = createAndSendPackage(cpuStruct->socketClientKernel,
	COD_SET_SHARED_VAR, sizeOf_SetSharedVar(object), buffer);
	destroySetSharedVar(object);
	free(buffer);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpuStruct->socketClientKernel);
		if (package != NULL) {
			int value;
			switch (package->msgCode) {
			case COD_SET_SHARED_VAR_RESPONSE:
				value = deserialize_int(package->stream);
				destroyPackage(package);
				return value;
				break;
			default:
				logError(
						"Kernel respondio con un codigo de mensaje desconocido");
				setErrorFlag(FLAG_UNKNOWN_MESSAGE_CODE);
				break;
			}
		} else {
			logError("No se pudo recibir los datos del kernel");
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	} else {
		logError("No se pudo enviar la solicitud al kernel");
		setErrorFlag(FLAG_DISCONNECTED_KERNEL);
	}
	return NULL_VALUE;
}
