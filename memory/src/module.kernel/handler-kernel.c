/*
 * kernelRequest.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernel(memory_struct* memoryStruct) {

	Package* package;
	bool running = true;
	while (running) {

		package = createAndReceivePackage(memoryStruct->socketClientKernel);
		if (package != NULL) {
			handleKernelRequest(memoryStruct, package);
		} else {
			running = false;
			logError("Kernel cerro la conexion para FD: %d",
					memoryStruct->socketClientKernel);
		}
		destroyPackage(package);
	}

	close(memoryStruct->socketClientKernel);
}

void handleKernelRequest(memory_struct* newProcess, Package* package) {
	/**
	 * TODO completar con las funcionalidades que solicita el kernel
	 */

	switch (package->msgCode) {
	case COD_SAVE_NEW_PROGRAM:
		startNewProcess(package);
		break;
	default:
		logError("El kernel solicito una accion desconocida FD: %d Cod: %d",
				newProcess->socketClientKernel, package->msgCode);
		break;
	}
}
