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

	requestInfo->pid; // pid del nuevo proceso
	requestInfo->size; // cantidad de paginas solicitadas

	//todo reservar paginas
		//asignar las paginas
			//si esta todo ok responder al kernel con COD_NEW_PROCESS_RESPONSE_OK
			//si no se pudo responder con el mensaje adecuado ej: ERROR_MEMORY_FULL
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

void startNewProcessTest (int processId, int stackSize, int sourceCodeSize, memory_struct* memoryStruct) {
	int index = 0;
	int totalSize = sourceCodeSize + stackSize * memoryStruct->pageSize;
	while (totalSize > 0) {
		if (newMemoryPage (memoryStruct, processId, index++) == -1)
			return; // devolver error porque no hay espacio para pagina nueva
		totalSize-= memoryStruct->pageSize;
	}
}
/*
char* saveData () {

}
*/
