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
		exit(ERROR_NETWORK_DISCONNECTED);
	}
	destroyPackage(package);

	logInfo("Esperando respuesta de la memoria");
	package = createAndReceivePackage(kernel_struct->socketClientMemoria);
	if (package != NULL && package->msgCode == COD_PAGE_SIZE_RESPONSE) {
		logInfo("Seteando el tamaño de pagina");
		kernel_struct->pageSize = deserialize_int(package->stream);
	} else {
		logError("La memoria respondio con una accion no permitida.");
		exit(ERROR_MESSAGE_CODE_UNKNOWN);
	}

}

/**
 * calculo la cantidad de paginas a utilizar y hago la solicitud a la memoria
 * package contiene el codigo fuente
 */
void reservePagesForNewProcess(Process* process, Package* sourceCodePackage) {
	int pagesNumber;
	Package* tmpPackage;

	pagesNumber = sourceCodePackage->size / process->kernelStruct->pageSize; //como ambos estan bytes me da la cantidad de paginas
	//agrego la cantidad de paginas para el stack
	pagesNumber += process->kernelStruct->config->stack_size;

	t_AddPagesToProcess* content = create_t_AddPagesToProcess(process->pid,
			pagesNumber);

	tmpPackage = createAndSendPackage(
			process->kernelStruct->socketClientMemoria, COD_NEW_PROCESS_REQUEST,
			sizeof_t_AddPagesToProcess(), (char*) content);	//todo verificar que content se envie como char*

	if (tmpPackage == NULL) {
		//Función de mensaje de rechazo porque no se pudo reservar pag en memoria
		consoleResponseRepulseMessage(process);
		destroyPackage(tmpPackage);
		free(content);
		logError(
				"No se pudo solicitar la reserva de paginas para el proceso pid %d",
				process->pid);
		removeFromNEW(process);
		exit(ERROR_NETWORK_DISCONNECTED);
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
			break;
		case ERROR_MEMORY_FULL:
			//Función de mensaje de rechazo porque no se pudo reservar pag en memoria
			consoleResponseRepulseMessage(process);
			logInfo(
					"La memoria indica que no hay espacio para el proceso pid %d",
					process->pid);
			removeFromNEW(process);
			destroyPackage(tmpPackage);
			free(content);
			exit(ERROR_WITHOUT_RESOURCES);
			break;
		default:
			logError("La memoria envio un mensaje no esperado");
			destroyPackage(tmpPackage);
			free(content);
			exit(ERROR_MESSAGE_CODE_UNKNOWN);
			break;
		}
	} else {
		destroyPackage(tmpPackage);
		logError(
				"No se recibio la respuesta de la memoria para el proceso pid %d",
				process->pid);
		destroyPackage(tmpPackage);
		free(content);
		removeFromNEW(process);
		exit(ERROR_NETWORK_DISCONNECTED);
	}

	destroyPackage(tmpPackage);
}

void sendSourceCodeForNewProcess(Process* process, Package* sourceCodePackage) {
	Package* tmpPackage;
	u_int32_t offset = 0;
	u_int32_t sizeBuffer;
	t_PageBytes* content;
	int pagesNumber = sourceCodePackage->size / process->kernelStruct->pageSize;
	int index;

	for (index = 0; index < pagesNumber; ++index) {
		offset += process->kernelStruct->pageSize * index + 1;
		sizeBuffer =
				(sourceCodePackage->size - offset > sourceCodePackage->size) ?
						process->kernelStruct->pageSize :
						sourceCodePackage->size - offset;
		content = create_t_PageBytes(sizeBuffer,
				sourceCodePackage->stream + offset);
		content->pageNumber = index;
		content->offset = 0;
		content->pid = process->pid;

		logInfo(
				"Solicitando a la memoria que almacene para el pid %d en la pagina %d offset %d tamaño del buffer %d",
				content->pid, content->pageNumber, content->offset,
				content->size);
		createAndSendPackage(process->kernelStruct->socketClientMemoria,
		COD_SAVE_PAGE_BYTES_REQUEST, sizeof_t_PageBytes(content),
				(char*) content);//todo verificar que content se envie como char*

		if (tmpPackage == NULL) {
			destroyPackage(tmpPackage);
			destroy_t_PageBytes(content);
			logError(
					"No se pudo almacenar el codigo fuente en la memoria para el pid %d",
					process->pid);
			removeFromNEW(process);
			exit(ERROR_NETWORK_DISCONNECTED);
		}
		destroyPackage(tmpPackage);

		//me quedo a la espera de la aprobacion
		tmpPackage = createAndReceivePackage(
				process->kernelStruct->socketClientMemoria);
		if (tmpPackage != NULL) {
			switch (tmpPackage->msgCode) {
			case COD_SAVE_PAGE_BYTES_REQUEST:
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
				exit(ERROR_WITHOUT_RESOURCES);
				break;
			default:
				logError("La memoria envio un mensaje no esperado");
				destroyPackage(tmpPackage);
				destroy_t_PageBytes(content);
				exit(ERROR_MESSAGE_CODE_UNKNOWN);
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
			exit(ERROR_NETWORK_DISCONNECTED);
		}

		destroy_t_PageBytes(content);
		destroyPackage(tmpPackage);
	}

}
