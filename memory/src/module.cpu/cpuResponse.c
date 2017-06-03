/*
 * cpuResponse.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "cpuResponse.h"

void sendPageSizeToCPU(CPU* cpu, memory_struct* memoryStruct) {
	char* stream = serialize_int(memoryStruct->config->marco_size);
	Package* package;
	package = createAndSendPackage(cpu->fileDescriptor,
			COD_PAGE_SIZE_RESPONSE, sizeof(uint32_t), stream);
	if (package == NULL)
		logError("Fallo el envio del tamaño de frame hacia el kernel");
	free(stream);
	destroyPackage(package);
}
