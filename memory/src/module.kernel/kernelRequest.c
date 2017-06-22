/*
 * handler-kernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "kernelRequest.h"
/**
 * esta funcion es invocada cuando se solicita generar espacio para un nuevo programa
 */

void startNewProcess(Package* package, kernel* kernel) {

	t_AddPagesToProcess* requestInfo = (t_AddPagesToProcess*) package->stream;

	Package* outPackage;
	int status = assignNewPages(kernel->memoryStruct, requestInfo->pid,
			requestInfo->size);

	if (status == 0) {
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_NEW_PROCESS_RESPONSE_OK, 0, NULL);
		logInfo ("Proceso %i cargado en memoria", requestInfo->pid);
	}
	else {
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_MEMORY_FULL, 0, NULL);
		logError ("No se puede cargar el proceso %i en memoria. No hay espacio disponible", requestInfo->pid);
	}

	destroyPackage(outPackage);
}

void addNewPages(Package* package, kernel* kernel) {

	t_AddPagesToProcess* requestInfo = (t_AddPagesToProcess*) package->stream;

	Package* outPackage;
	int status = assignNewPages(kernel->memoryStruct, requestInfo->pid,
			requestInfo->size);

	if (status == 0)
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_ADD_PROCESS_PAGES_RESPONSE, 0, NULL);
	else
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_MEMORY_FULL, 0, NULL);

	destroyPackage(outPackage);
}

void freeMemoryPage (Package* package, kernel* kernel) {
	t_FreePageToProcess* freePageToProcess = (t_FreePageToProcess*) package->stream;
	Package* outPackage;

	freePage (kernel->memoryStruct, freePageToProcess->pid, freePageToProcess->page);
	outPackage = createAndSendPackage (kernel->fileDescriptor, COD_FREE_PAGE_RESPONSE, 0, NULL);

	destroyPackage (outPackage);
}

void saveData(Package* package, kernel* kernel) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;
	int status = processWrite(kernel->memoryStruct, pageBytes);

	if (status == 0) {
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_SAVE_PAGE_BYTES_RESPONSE, 0, NULL);
		logTrace ("Se almacenan los datos enviados por Kernel para pid: %i pag: %i offset: %i size: %i", pageBytes->pid,
				pageBytes->pageNumber, pageBytes->offset, pageBytes->size);
	}
	else {
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);
		logError ("La peticion de escritura de Kernel produce segmentation fault");
	}

	destroyPackage(outPackage);
	destroy_t_PageBytes(pageBytes);
}

void readData(Package* package, kernel* kernel) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;

	int status = processRead(kernel->memoryStruct, pageBytes);

	if (status == 0) {
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_GET_PAGE_BYTES_RESPONSE, package->size,
				serialize_t_PageBytes (pageBytes));
		logTrace ("Se devuelven los datos pedidos por Kernel para pid: %i pag: %i offset: %i size: %i", pageBytes->pid,
				pageBytes->pageNumber, pageBytes->offset, pageBytes->size);
	}
	else {
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);
		logError ("La peticion de lectura de Kernel produce segmentation fault");
	}

	destroy_t_PageBytes(pageBytes);
	destroyPackage(outPackage);
}

void endProcess(Package* package, kernel* kernel) {
	int* pid = (int*) package->stream;

	logTrace ("Memoria recibe solicitud para terminar con proceso %i", pid);
	Package* outPackage;
	terminateProcess(kernel->memoryStruct, *pid);
	outPackage = createAndSendPackage (kernel->fileDescriptor, COD_END_PROCESS_RESPONSE, 0, NULL);

	destroyPackage (outPackage);
}
