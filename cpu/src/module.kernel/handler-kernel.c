/*
 * handler-kernel.c
 *
 *  Created on: 15/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernel(cpu_struct *cpuStruct) {

	Package* package;
	while (true) {

		logInfo("Esperando instrucciones del kernel");
		package = createAndReceivePackage(cpuStruct->socketClientKernel);

		if (package != NULL)
			handleKernelRequest(cpuStruct, package);
		else {
			logError("El kernel cerro la conexion para FD: %d",
					cpuStruct->socketClientKernel);
			exit(ERROR_DISCONNECTED_SOCKET);
		}

		destroyPackage(package);
	}
}

void handleKernelRequest(cpu_struct* cpuStruct, Package* package) {

	switch (package->msgCode) {
	case COD_SET_STACK_PAGE_SIZE: //para actualizar el tamaño de stack
		updateStackPageSize(cpuStruct, package);
		break;
	case COD_EXEC_NEW_PCB: //se envia a ejecutar un nuevo pcb
		executeNewPcb(cpuStruct, package);
		break;
	case COD_CONTINUE_EXECUTION: //paso un quantum y se decidio continuar ejecytando el mismo pcb
		continueExecution(cpuStruct, package);
		break;
	case COD_CONTEXT_SWITCH_REQUEST: //se solicito desalojar el pcb
		contextSwitch(cpuStruct, package);
		break;
	default:
		logError("El kernel solicito una accion no permitida");
		exit(ERROR_UNKNOWN_MESSAGE_CODE);
		break;
	}
}
