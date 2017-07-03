/*
 * kernelResponse.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "kernelResponse.h"

//SURGE AL FINALIZAR LA EJECUCION DE UNA INSTRUCCION
void reportEndInstruction(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel que la instruccion se ejecutó correctamente");
	Package* package = createAndSendPackage(cpuStruct->socketClientKernel,
	COD_END_INSTRUCCION, 0, NULL);
	destroyPackage(package);
	//NO SE DESCARGA EL PCB PORQUE PUEDE VENIR UNA SOLICITUD PARA QUE SIGA EJECUTANDO EL MISMO PROCESO
	//TAMPOCO HACE FALTA ENVIARLO AL KERNEL (MENOS OVERHEAD -> EL KERNEL PARA CAMBIAR DE CONTEXTO DEBERIA INVOCAR CON CONTEXTsWITCH)
}

//SURGE CUANDO LA CPU SE VA DESCONECTAR
void reportCpuDisconected(cpu_struct* cpuStruct) {
	logInfo(
			"Informando al kernel que la instruccion se ejecutó correctamente, y que la cpu dejara de dar servicio");
	sendPCB(cpuStruct, COD_SIGNAL_DISCONNECTED);
	unloadPCB(cpuStruct);
}

//SURGE CUANDO SE EJECUTA LA INSTRUCCION ANSISOP DE FIN DE PROGRAMA
//O CUANDO LLEGO AL MAXIMO PROGRAM COUNTER CONFIGURADO EN LA METADATA
void reportEndProcess(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel que el programa finalizo con éxito");
	sendPCB(cpuStruct, COD_PROGRAM_FINISHED);
	unloadPCB(cpuStruct);
}

//SE INVOCA CUANDO EL KERNEL SOLICITA EL CONTEXT SWITCH O CUANDO EL PROCESO VA A BLOQUEARSE
void reportContextSwich(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel en nuevo pcb producto del context switch");
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
	free(buffer);
	destroyPackage(package);
}

void reportExcecutionError(cpu_struct* cpuStruct, int errorFlag) {
	Package* package = NULL;
	char* buffer;
	size_t size = sizeof(uint32_t);
	int socket = cpuStruct->socketClientKernel;
	switch (errorFlag) {
	case FLAG_STACKOVERFLOW:
	case FLAG_SEGMENTATION_FAULT:
	case FLAG_DISCONNECTED_MEMORY:
		buffer = serialize_int(SC_ERROR_MEMORY_EXCEPTION);
		package = createAndSendPackage(socket, COD_EXECUTION_ERROR, size,
				buffer);
		free(buffer);
		if(errorFlag==FLAG_DISCONNECTED_MEMORY){
			exit(EXIT_SUCCESS);//no puede operar sin memoria
		}
		break;
	case FLAG_DISCONNECTED_KERNEL:
		logError(
				"El kernel esta desconectado, no puedo continuar ejecuarnado por lo tanto finalizo tambien");
		logTrace(
				"Finalizando proceso cpu porque no se pudo conectar con el kernel");
		exit(EXIT_SUCCESS);
		break;
	case FLAG_UNKNOWN_MESSAGE_CODE:
		logError(
				"El kernel no esta respondiendo adecuadamente, no puedo continuar ejecuarnado");
		logTrace("Finalizando proceso cpu porque hay errores en el kernel");
		exit(EXIT_SUCCESS);
		break;
	case FLAG_UNKNOWN_ERROR:
	default:
		buffer = serialize_int(SC_ERROR_UNKNOWN);
		package = createAndSendPackage(socket, COD_EXECUTION_ERROR, size,
				buffer);
		free(buffer);
		break;
	}
	destroyPackage(package);
}

int getSharedVarriableValue(cpu_struct* cpuStruct, char* name) {
	Package* package;
	int newValue = NULL_VALUE;
	package = createAndSendPackage(cpuStruct->socketClientKernel,
	COD_GET_SHARED_VAR, strlen(name), name);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpuStruct->socketClientKernel);
		if (package != NULL) {
			if (package->msgCode == COD_SYSCALL_SUCCESS) {
				newValue = deserialize_int(package->stream);
				logTrace("Se obtuvo el valor de la variable: %s valor: %d",
						name, newValue);
			} else {
				setErrorFlag(FLAG_SYSCALL_FAILURE);
			}
			destroyPackage(package);
		} else {
			logError("No se pudo recibir los datos del kernel");
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	} else {
		logError("No se pudo enviar la solicitud al kernel");
		setErrorFlag(FLAG_DISCONNECTED_KERNEL);
	}
	return newValue;
}

int setSharedVarriableValue(cpu_struct* cpuStruct, char* name, int value) {
	Package* package;
	int newValue = NULL_VALUE;
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
			if (package->msgCode == COD_SYSCALL_SUCCESS) {
				newValue = deserialize_int(package->stream);
				logTrace(
						"Se actualizo el valor de la variable: %s con el valor: %d",
						name, newValue);
				return newValue;
			} else {
				setErrorFlag(FLAG_SYSCALL_FAILURE);
			}
			destroyPackage(package);
		} else {
			logError("No se pudo recibir los datos del kernel");
			setErrorFlag(FLAG_DISCONNECTED_KERNEL);
		}
	} else {
		logError("No se pudo enviar la solicitud al kernel");
		setErrorFlag(FLAG_DISCONNECTED_KERNEL);
	}
	return newValue;
}
