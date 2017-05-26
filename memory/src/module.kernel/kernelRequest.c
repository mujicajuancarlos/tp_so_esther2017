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

void startNewProcess(Package* package, memory_struct* memoryStruct){

	t_AddPagesToProcess* requestInfo = package->stream;

	Package* outPackage;
	int status = assignNewPages(memoryStruct, requestInfo->pid, requestInfo->size);

	if (status == 0)
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, COD_NEW_PROCESS_RESPONSE_OK, 0, NULL);
	else
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, ERROR_MEMORY_FULL, 0, NULL);

	destroyPackage (outPackage);
}

void addNewPages (Package* package, memory_struct* memoryStruct) {

	t_AddPagesToProcess* requestInfo = package->stream;

	Package* outPackage;
	int status = assignNewPages (memoryStruct, requestInfo->pid, requestInfo->size);

	if (status == 0)
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, COD_ADD_PROCESS_PAGES_RESPONSE, 0, NULL);
	else
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, ERROR_MEMORY_FULL, 0, NULL);

	destroyPackage (outPackage);
}

void sendPageSize(memory_struct* memoryStruct){
	char* stream = serialize_int(memoryStruct->config->marco_size);
	Package* package;
	package = createAndSendPackage(memoryStruct->socketClientKernel, COD_PAGE_SIZE_RESPONSE,sizeof(int), stream);
	if(package == NULL)
		logError("Fallo el envio del tamaño de frame hacia el kernel");
	free(stream);
	destroyPackage(package);
}

void startNewProcessTest (int processId, int pages, memory_struct* memoryStruct) {
	Package* outPackage;
	int status = assignNewPages (memoryStruct, processId, pages);

	if (status == 0)
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, COD_NEW_PROCESS_RESPONSE_OK, 0, NULL);
	else
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, ERROR_MEMORY_FULL, 0, NULL);

	destroyPackage (outPackage);
}

void saveData (Package* package, memory_struct* memoryStruct) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes (package->stream);
	Package* outPackage;
	int status = processWrite (memoryStruct, pageBytes);

	if (status == 0)
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, COD_SAVE_PAGE_BYTES_RESPONSE, 0, NULL);
	else
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroyPackage (outPackage);
	destroy_t_PageBytes (pageBytes);
}

void readData (Package* package, memory_struct* memoryStruct) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes (package->stream);
	Package* outPackage;

	int status = processRead (memoryStruct, pageBytes);

	if (status == 0)
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, COD_GET_PAGE_BYTES_RESPONSE, pageBytes->size, pageBytes->buffer);
	else
		outPackage = createAndSendPackage (memoryStruct->socketClientKernel, ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroy_t_PageBytes(pageBytes);
	destroyPackage (outPackage);
}

void endProcess (memory_struct* memoryStruct, int pid) {
	freeProcess (memoryStruct, pid);
}
