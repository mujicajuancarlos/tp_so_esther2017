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

void handleKernelRequest(memory_struct* memory_struct, Package* package) {
	/**
	 * TODO completar con las funcionalidades que solicita el kernel
	 */

	switch (package->msgCode) {
	case COD_PAGE_SIZE_REQUEST: //para pedir el tamaño de pagina
		sendPageSize(memory_struct);
		break;
	case COD_NEW_PROCESS_REQUEST: //para solicitar la reserva de n paginas para un proceso nuevo
		startNewProcess(package, memory_struct);
		break;
	case COD_SAVE_PAGE_BYTES_REQUEST: //para guardar bytes en una pagina
		//todo realizar la funcion
		break;
	case COD_ADD_PROCESS_PAGES_REQUEST: //para agregar paginas adicionales a un proceso existente
		//todo realizar la funcion
		break;
	case COD_END_PROCESS_REQUEST: //para liberar paginas de un proceso que finalizo
		//todo realizar la funcion
		break;
	default:
		logError("El kernel solicito una accion desconocida FD: %d Cod: %d",
				memory_struct->socketClientKernel, package->msgCode);
		break;
	}
}
