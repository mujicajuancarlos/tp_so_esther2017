/*
 * kernelRequest.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernel(kernel* kernel) {
	Package* package;
	bool running = true;
	while (running) {
		package = createAndReceivePackage(kernel->fileDescriptor);
		if (package != NULL) {
			handleKernelRequest(kernel, package);
		} else {
			running = false;
			logError("Kernel cerro la conexion para FD: %d",
					kernel->fileDescriptor);
		}
		destroyPackage(package);
	}
	kernel->memoryStruct->socketClientKernel = -1;
	close(kernel->fileDescriptor);
}

void handleKernelRequest(kernel* kernel, Package* package) {
	/**
	 * TODO completar con las funcionalidades que solicita el kernel
	 */

	switch (package->msgCode) {
	case COD_PAGE_SIZE_REQUEST: //para pedir el tamaño de pagina
		sendPageSizeToKernel(kernel);
		break;
	case COD_NEW_PROCESS_REQUEST: //para solicitar la reserva de n paginas para un proceso nuevo
		startNewProcess(package, kernel);
		break;
	case COD_SAVE_PAGE_BYTES_REQUEST: //para guardar bytes en una pagina
		saveData(package, kernel);
		break;
	case COD_GET_PAGE_BYTES_REQUEST: // para leer bytes de una pagina
		readData(package, kernel);
		break;
	case COD_ADD_PROCESS_PAGES_REQUEST: //para agregar paginas adicionales a un proceso existente
		addNewPages(package, kernel);
		break;
	case COD_FREE_PAGE_REQUEST:
		freeMemoryPage (package, kernel);
		break;
	case COD_END_PROCESS_REQUEST: //para liberar todas las paginas de un proceso que finalizo
		endProcess (package, kernel);
		break;
	default:
		logError("El kernel solicito una accion desconocida FD: %d Cod: %d",
				kernel->fileDescriptor, package->msgCode);
		break;
	}
}
