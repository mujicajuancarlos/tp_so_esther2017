/*
 * kernelResponse.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "kernelResponse.h"

void sendPageSizeToKernel(kernel* kernel) {
	char* stream = serialize_int(kernel->memoryStruct->config->marco_size);
	Package* package;
	package = createAndSendPackage(kernel->fileDescriptor,
			COD_PAGE_SIZE_RESPONSE, sizeof(int), stream);
	if (package == NULL)
		logError("Fallo el envio del tamaño de frame hacia el kernel");
	free(stream);
	destroyPackage(package);
}
