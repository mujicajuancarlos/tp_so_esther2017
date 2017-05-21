/*
 * memoryManager.c
 *
 *  Created on: 19/5/2017
 *      Author: utnso
 */

#include "memoryRequests.h"
/**
 * mi objetivo es solicitar y setear el tmaño de pagina
 */
void loadMemoryPageSize(kernel_struct* kernel_struct) {
	Package* package;
	logInfo("Solicitando a la memoria el tamaño de pagina");
	package = createAndSendPackage(kernel_struct->socketClientMemoria,
	COD_PAGE_SIZE_REQUEST, 0,
	NULL);
	if (package == NULL) {
		logError("No se pudo solicitar el tamaño de pagina");
		exit(-1); //todo ver que codigo de error se asiga en base a la tabla de errores
	}
	destroyPackage(package);

	logInfo("Esperando respuesta de la memoria");
	package = createAndReceivePackage(kernel_struct->socketClientMemoria);
	if (package != NULL && package->msgCode == COD_PAGE_SIZE_RESPONSE) {
		logInfo("Seteando el tamaño de pagina");
		kernel_struct->pageSize = deserialize_int(package->stream);
	} else {
		logError("La memoria respondio con una accion no permitida.");
		exit(-1); //todo ver que codigo de error se asiga en base a la tabla de errores
	}

}

/**
 * calculo la cantidad de paginas a utilizar y hago la solicitud a la memoria
 * package contiene el codigo fuente
 */
void reservePagesForNewProcess(Process* process, Package* package) {
	int pagesNumber;
	Package* tmpPackage;

	pagesNumber = package->size / process->kernelStruct->pageSize; //como ambos estan bytes me da la cantidad de paginas
	//agrego la cantidad de paginas para el stack
	pagesNumber += process->kernelStruct->config->stack_size;

	t_AddPagesToProcess* content = create_t_AddPagesToProcess(process->pid,
			pagesNumber);

	tmpPackage = createAndSendPackage(
			process->kernelStruct->socketClientMemoria, COD_NEW_PROCESS_REQUEST,
			sizeof_t_AddPagesToProcess(), content);

	if (tmpPackage == NULL) {
		destroyPackage(tmpPackage);
		logError(
				"No se puedo solicitar la reserva de paginas para el proceso pid %d",
				process->pid);
		//todo remover de la lista de nuevos informar a consola
		exit(-1); //todo ver que codigo de error se asiga en base a la tabla de errores
	}

	destroyPackage(tmpPackage);
	logInfo(
			"Se envio la solicitud para reservar %d paginas para el proceso pid %d",
			pagesNumber, process->pid);
	tmpPackage = createAndReceivePackage(
			process->kernelStruct->socketClientMemoria);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_NEW_PROCESS_RESPONSE:
			logInfo("La memoria reservo las paginas para el proceso pid %d", process->pid);
			break;
		case ERROR_MEMORY_FULL:
			logInfo(
					"La memoria indica que no hay espacio para el proceso pid %d",
					process->pid);
			//todo remover de la lista de nuevos informar a consola
			exit(-1);
			break;
		default:
			logError("La memoria envio un mensaje no esperado");
			break;
		}
	}

	destroyPackage(tmpPackage);
}

void sendSourceCodeForNewProcess(Process* process, Package* package) {
	//todo pendiente de desarrollo
}
