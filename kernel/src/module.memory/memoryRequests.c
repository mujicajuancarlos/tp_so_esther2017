/*
 * memoryManager.c
 *
 *  Created on: 19/5/2017
 *      Author: utnso
 */

#include "memoryRequests.h"

pthread_mutex_t memoryRequestMutex;

void initializeMemoryRequestMutex() {
	pthread_mutex_init(&memoryRequestMutex, NULL);
}

void destroyMemoryRequestMutex() {
	pthread_mutex_destroy(&memoryRequestMutex);
}

void memoryRequestMutex_lock() {
	pthread_mutex_lock(&memoryRequestMutex);
}

void memoryRequestMutex_unlock() {
	pthread_mutex_unlock(&memoryRequestMutex);
}
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
	memoryRequestMutex_lock();
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
	memoryRequestMutex_unlock();
}

void freePageForProcess(Process* process, int memoryPageNumber, int* status) {
	memoryRequestMutex_lock();
	Package* tmpPackage;
	t_FreePageToProcess* content = create_t_FreePageToProcess(process->pid,
			memoryPageNumber);
	tmpPackage = createAndSendPackage(
			process->kernelStruct->socketClientMemoria,
			COD_FREE_PAGE_REQUEST, sizeof_t_FreePageToProcess(),
			(char*) content);
	free(content);
	if (tmpPackage == NULL) {
		logError("La memoria no esta conectada");
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo("Se solicitó liberar la pagina: %d para el pid: %d",
			memoryPageNumber, process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(
			process->kernelStruct->socketClientMemoria);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_FREE_PAGE_RESPONSE:
			logInfo("La memoria libero la pagina %d para el proceso pid %d",
					memoryPageNumber, process->pid);
			*status = FREE_MEMORY_SUCCES;
			break;
		default:
			logError("La memoria envio un mensaje no esperado");
			exit(EXIT_FAILURE);
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("La memoria no esta conectada");
		exit(EXIT_FAILURE);
	}
	memoryRequestMutex_unlock();
}

/**
 * calculo la cantidad de paginas a utilizar y hago la solicitud a la memoria
 * package contiene el codigo fuente
 */
void reservePagesForNewProcess(Process* process, Package* sourceCodePackage) {
	memoryRequestMutex_lock();
	Package* tmpPackage;
	int pagesNumber =
			(sourceCodePackage->size % process->kernelStruct->pageSize) ? 1 : 0;
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
		logError(
				"No se pudo solicitar la reserva de paginas para el proceso pid %d",
				process->pid);
		exit(EXIT_FAILURE);	//tiene sentido por que la memoria no responde
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
			break;
		default:
			logError(
					"La memoria envio un mensaje no esperado despues de la solicitud 'Reservar paginas para proceso %d'",
					process->pid);
			exit(EXIT_FAILURE);
			break;
		}
	} else {
		logError(
				"La memoria cerro la conexion despues de la solicitud 'Reservar paginas para proceso %d'",
				process->pid);
		exit(EXIT_FAILURE);	//tiene sentido por que la memoria no responde
	}
	destroyPackage(tmpPackage);
	memoryRequestMutex_unlock();
}

/**
 * usado cuando un proceso es enviado a EXIT
 */
void notifyEndProcessToMemory(Process* process) {
	memoryRequestMutex_lock();
	char* buffer = serialize_int(process->pid);
	Package* tmpPackage = createAndSendPackage(
			process->kernelStruct->socketClientMemoria, COD_END_PROCESS_REQUEST,
			sizeof(uint32_t), buffer);
	free(buffer);
	if (tmpPackage == NULL) {
		logError(
				"No se pudo solicitar la liberacion de paginas del proceso pid %d",
				process->pid);
		exit(EXIT_FAILURE);	//tiene sentido por que la memoria no responde
	}
	destroyPackage(tmpPackage);
	logInfo("Se envio la solicitud para liberar las paginas del proceso pid %d",
			process->pid);

	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(
			process->kernelStruct->socketClientMemoria);
	if (tmpPackage != NULL) {
		if (tmpPackage->msgCode == COD_END_PROCESS_RESPONSE) {
			logInfo("La memoria libero las paginas del proceso %d", process->pid);
		} else {
			logError(
					"La memoria respondio con un codigo no esperado despues de la solicitud 'Liberar paginas para proceso %d'",
					process->pid);
			exit(EXIT_FAILURE);
		}
	} else {
		logError(
				"La memoria cerro la conexion despues de la solicitud 'Liberar paginas para proceso %d'",
				process->pid);
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	memoryRequestMutex_unlock();
}

void sendSourceCodeForNewProcess(Process* process, Package* sourceCodePackage) {

	bool hasError = false;

	logInfo("Enviando el codigo fuente a la memoria para el pid: %d",
			process->pid);
	saveDataOnMemory(process, 0, 0, sourceCodePackage->size,
			sourceCodePackage->stream, &hasError);//startPage 0 porque se trata del codigo
	logInfo("Se envio el codigo fuente a la memoria para el pid: %d",
			process->pid);
	if (hasError) {
		logError(
				"No se pudo almacenar el codigo fuente para el proceso pid: %d, se procede a eliminarlo del sistema",
				process->pid);
		consoleResponseRepulseMessage(process);
	}

}

/**
 * Soy una funcion generica que sirve para guardar datos en la memoria
 * SIN IMPORTAR QUE ESOS DATOS ESTEN PARTIDOS ENTRE DOS O MAS PAGINAS CONTIGUAS
 * ---importante -> hay que pasarle un numero de pagina de inicio -> startPage
 */
void saveDataOnMemory(Process* process, int startPage, u_int32_t offset,
		t_size length, char* buffer, bool* hasError) {
	int bufferOffset = 0;
	char* tmpBuffer;
	int firstByte, lastByte, pageNumber, tmpBufferSize, pageSize;
	pageSize = process->kernelStruct->pageSize;
	int firstPage = startPage + (offset / pageSize);
	int lastPage = startPage + ((offset + length) / pageSize);
	int firstPageOffset = offset % pageSize;
	int lastPageOffset = (offset + length) % pageSize;
	for (pageNumber = firstPage; !*hasError && pageNumber <= lastPage;
			++pageNumber) {
		firstByte = (pageNumber == firstPage) ? firstPageOffset : 0;
		lastByte = (pageNumber == lastPage) ? lastPageOffset : pageSize;
		tmpBufferSize = lastByte - firstByte;
		tmpBuffer = malloc(tmpBufferSize);
		memcpy(tmpBuffer, buffer + bufferOffset, tmpBufferSize);
		saveDataOnPage(process, pageNumber, firstByte, tmpBufferSize, tmpBuffer,
				hasError);
		if (!*hasError) {
			bufferOffset += tmpBufferSize;
		}
		free(tmpBuffer);
	}
}

void saveDataOnPage(Process* process, int pageNumber, int offset, int size,
		char* buffer, bool* hasError) {
	memoryRequestMutex_lock();
	Package* package;
	t_PageBytes* data = create_t_PageBytes(process->pid, pageNumber, offset,
			size, buffer);
	char* serializedData = serialize_t_PageBytes(data);
	size_t serializedSize = sizeof_t_PageBytes(data);
	logTrace("Enviando datos del pid: %d pag: %d offset: %d size: %d",
			process->pid, pageNumber, offset, size);
	package = createAndSendPackage(process->kernelStruct->socketClientMemoria,
	COD_SAVE_PAGE_BYTES_REQUEST, serializedSize, serializedData);
	free(serializedData);
	destroy_t_PageBytes(data);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(
				process->kernelStruct->socketClientMemoria);
		if (package != NULL) {
			switch (package->msgCode) {
			case COD_SAVE_PAGE_BYTES_RESPONSE:
				logInfo(
						"Solicitud para guardar datos realizada para pid: %d pag: %d offset: %d size: %d",
						process->pid, pageNumber, offset, size);
				break;
			case ERROR_SEGMENTATION_FAULT:
				logError(
						"La memoria informo que la solicitud produjo SegmentationFault");
				*hasError = true;
				break;
			default:
				logError("La memoria respondio con un codigo no esperado");
				*hasError = true;
				break;
			}
			destroyPackage(package);
		} else {
			logError("No se pudo recibir la solicitud a la memoria");
			*hasError = true;
		}
	} else {
		logError("No se pudo enviar la solicitud a la memoria");
		*hasError = true;
	}
	memoryRequestMutex_unlock();
}
