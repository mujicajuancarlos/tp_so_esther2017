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
		exit(EXIT_FAILURE);
	}
	destroyPackage(package);

	logInfo("Esperando respuesta de la memoria");
	package = createAndReceivePackage(kernel_struct->socketClientMemoria);
	if (package != NULL && package->msgCode == COD_PAGE_SIZE_RESPONSE) {
		logInfo("Seteando el tamaño de pagina");
		kernel_struct->pageSize = deserialize_int(package->stream);
		destroyPackage(package);
	} else {
		logError("La memoria respondio con una accion no permitida.");
		exit(EXIT_FAILURE);
	}
}

void reserveNewHeapPageForProcess(Process* process, int* status) {
	Package* tmpPackage;
	t_AddPagesToProcess* content = create_t_AddPagesToProcess(process->pid, 1);
	tmpPackage = createAndSendPackage(
			process->kernelStruct->socketClientMemoria,
			COD_ADD_PROCESS_PAGES_REQUEST, sizeof_t_AddPagesToProcess(),
			(char*) content);
	free(content);
	if (tmpPackage == NULL) {
		logError("La memoria no esta conectada %d", process->pid);
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo("Se solicitó una nueva pagina para el proceso pid %d",
			process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(
			process->kernelStruct->socketClientMemoria);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_ADD_PROCESS_PAGES_RESPONSE:
			logInfo("La memoria reservo la nueva pagina para el proceso pid %d",
					process->pid);
			*status = MALLOC_MEMORY_SUCCES;
			break;
		case ERROR_MEMORY_FULL:
			logInfo(
					"La memoria indico que no hay un pagina disponible para el proceso pid %d",
					process->pid);
			*status = SC_ERROR_ADD_PAGE_REFUSED;
			break;
		default:
			logError("La memoria envio un mensaje no esperado");
			*status = SC_ERROR_MEMORY_EXCEPTION;
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("La memoria no esta conectada");
		exit(EXIT_FAILURE);
	}
}

/**
 * calculo la cantidad de paginas a utilizar y hago la solicitud a la memoria
 * package contiene el codigo fuente
 */
void reservePagesForNewProcess(Process* process, Package* sourceCodePackage) {
	Package* tmpPackage;
	bool hasOffset = (sourceCodePackage->size % process->kernelStruct->pageSize)
			> 0;
	int pagesNumber = hasOffset ? 1 : 0;
	pagesNumber += sourceCodePackage->size / process->kernelStruct->pageSize; //como ambos estan bytes me da la cantidad de paginas
	//agrego la cantidad de paginas para el stack
	pagesNumber += process->kernelStruct->config->stack_size;

	t_AddPagesToProcess* content = create_t_AddPagesToProcess(process->pid,
			pagesNumber);
	tmpPackage = createAndSendPackage(
			process->kernelStruct->socketClientMemoria, COD_NEW_PROCESS_REQUEST,
			sizeof_t_AddPagesToProcess(), (char*) content);
	free(content);
	if (tmpPackage == NULL) {
		//Función de mensaje de rechazo porque no se pudo reservar pag en memoria
		consoleResponseRepulseMessage(process);
		logError(
				"No se pudo solicitar la reserva de paginas para el proceso pid %d",
				process->pid);
		exit(EXIT_FAILURE);
	}

	destroyPackage(tmpPackage);
	logInfo(
			"Se envio la solicitud para reservar %d paginas para el proceso pid %d",
			pagesNumber, process->pid);

	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(
			process->kernelStruct->socketClientMemoria);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_NEW_PROCESS_RESPONSE_OK:
			logInfo("La memoria reservo las paginas para el proceso pid %d",
					process->pid);
			runProgramIsOK_response(process);
			break;
		case ERROR_MEMORY_FULL:
			//Función de mensaje de rechazo porque no se pudo reservar pag en memoria
			consoleResponseRepulseMessage(process);
			logInfo(
					"La memoria indica que no hay espacio para el proceso pid %d",
					process->pid);
			exit(EXIT_FAILURE);
			break;
		default:
			logError("La memoria envio un mensaje no esperado");
			consoleResponseRepulseMessage(process);
			exit(EXIT_FAILURE);
			break;
		}
	} else {
		logError(
				"No se recibio la respuesta de la memoria para el proceso pid %d",
				process->pid);
		consoleResponseRepulseMessage(process);
		exit(EXIT_FAILURE);
	}

	destroyPackage(tmpPackage);
}

void sendSourceCodeForNewProcess(Process* process, Package* sourceCodePackage) {
	Package* tmpPackage;
	u_int32_t offset = 0;
	u_int32_t sizeBuffer;
	t_PageBytes* content;
	bool hasOffset = (sourceCodePackage->size % process->kernelStruct->pageSize)
			> 0;
	int pagesNumber = hasOffset ? 1 : 0;
	pagesNumber += sourceCodePackage->size / process->kernelStruct->pageSize;
	int index;

	for (index = 0; index < pagesNumber; ++index) {
		offset += process->kernelStruct->pageSize * index + 1;
		sizeBuffer =
				(sourceCodePackage->size - offset > sourceCodePackage->size) ?
						process->kernelStruct->pageSize :
						sourceCodePackage->size - offset;
		content = create_t_PageBytes(process->pid, index, 0, sizeBuffer,
				sourceCodePackage->stream + offset);
		content->pageNumber = index;
		content->offset = 0;
		content->pid = process->pid;

		logInfo(
				"Solicitando a la memoria que almacene para el pid %d en la pagina %d offset %d tamaño del buffer %d",
				content->pid, content->pageNumber, content->offset,
				content->size);
		tmpPackage = createAndSendPackage(
				process->kernelStruct->socketClientMemoria,
				COD_SAVE_PAGE_BYTES_REQUEST, sizeof_t_PageBytes(content),
				(char*) content);
		if (tmpPackage == NULL) {
			destroyPackage(tmpPackage);
			destroy_t_PageBytes(content);
			logError(
					"No se pudo almacenar el codigo fuente en la memoria para el pid %d",
					process->pid);
			removeFromNEW(process);
			exit(EXIT_FAILURE);
		}
		destroyPackage(tmpPackage);

		//me quedo a la espera de la aprobacion
		tmpPackage = createAndReceivePackage(
				process->kernelStruct->socketClientMemoria);
		if (tmpPackage != NULL) {
			switch (tmpPackage->msgCode) {
			case COD_SAVE_PAGE_BYTES_RESPONSE:
				logInfo(
						"La memoria almaceno correctamente la pagina %d para el pid %d",
						content->pageNumber, content->pid);
				break;
			case ERROR_MEMORY_FULL:
				logInfo(
						"La memoria indica que no hay espacio para el proceso pid %d",
						process->pid);
				destroyPackage(tmpPackage);
				destroy_t_PageBytes(content);
				removeFromNEW(process);
				exit(EXIT_FAILURE);
				break;
			default:
				logError("La memoria envio un mensaje no esperado");
				destroyPackage(tmpPackage);
				destroy_t_PageBytes(content);
				exit(EXIT_FAILURE);
				break;
			}
		} else {
			destroyPackage(tmpPackage);
			logError(
					"No se recibio la respuesta de la memoria para el proceso pid %d",
					process->pid);
			destroyPackage(tmpPackage);
			destroy_t_PageBytes(content);
			removeFromNEW(process);
			exit(EXIT_FAILURE);
		}

		destroy_t_PageBytes(content);
		destroyPackage(tmpPackage);
	}

}
