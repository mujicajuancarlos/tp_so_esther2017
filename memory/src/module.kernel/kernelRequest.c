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
	t_new_sourceCode_request* request = deserialize_new_sourceCode_request(package->stream);

	/* en realidad aca hay una cuenta que debería estar haciendo el kernel. kernel debería simplementer informar el numero de paginas que quiere
	 * ya sean stack, heap... a la memoria no le interesa para que las va a usar, para la memoria todas las paginas son iguales */

	int index = 0;
	int totalSize = request->sourceCodeSize + request->stackSize * memoryStruct->pageSize;
	while (totalSize > 0) {
		if (newMemoryPage (memoryStruct, request->pid, index++) == -1)
			return; // devolver error porque no hay espacio para pagina nueva
		totalSize-= memoryStruct->pageSize;
	}

	destroy_new_sourceCode_request(request);
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
