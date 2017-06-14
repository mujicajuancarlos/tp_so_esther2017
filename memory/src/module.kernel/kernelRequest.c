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

	if (status == 0)
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_NEW_PROCESS_RESPONSE_OK, 0, NULL);
	else
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_MEMORY_FULL, 0, NULL);

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

void saveData(Package* package, kernel* kernel) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;
	int status = processWrite(kernel->memoryStruct, pageBytes);

	if (status == 0)
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_SAVE_PAGE_BYTES_RESPONSE, 0, NULL);
	else
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroyPackage(outPackage);
	destroy_t_PageBytes(pageBytes);
}

void readData(Package* package, kernel* kernel) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;

	int status = processRead(kernel->memoryStruct, pageBytes);

	if (status == 0)
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				COD_GET_PAGE_BYTES_RESPONSE, pageBytes->size,
				serialize_t_PageBytes (pageBytes));
	else
		outPackage = createAndSendPackage(kernel->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroy_t_PageBytes(pageBytes);
	destroyPackage(outPackage);
}

void endProcess(kernel* kernel, int pid) {
	freeProcess(kernel->memoryStruct, pid);
}
