/*
 * cpuResponse.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "cpuResponse.h"

void sendPageSizeToCPU(CPU* cpu) {
	char* stream = serialize_int(cpu->memoryStruct->config->marco_size);
	Package* package;
	package = createAndSendPackage(cpu->fileDescriptor,
			COD_PAGE_SIZE_RESPONSE, sizeof(uint32_t), stream);
	if (package == NULL)
		logError("Fallo el envio del tamaño de frame hacia el cpu");
	free(stream);
	destroyPackage(package);
}
