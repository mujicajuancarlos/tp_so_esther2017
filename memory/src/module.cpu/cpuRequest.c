/*
 * cpuRequest.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "cpuRequest.h"

void cpuSaveData(CPU* cpu, Package* package) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;
	int status = processWrite(cpu->memoryStruct, pageBytes);

	if (status == 0) {
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_SAVE_PAGE_BYTES_RESPONSE, 0, NULL);
		logTrace ("Se almacenan los datos enviados por CPU para pid: %i pag: %i offset: %i size: %i", pageBytes->pid,
				pageBytes->pageNumber, pageBytes->offset, pageBytes->size);
	}
	else {
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);
		logError ("La peticion de escritura de CPU produce segmentation fault");
	}

	destroyPackage(outPackage);
	destroy_t_PageBytes(pageBytes);
}

void cpuReadData(CPU* cpu, Package* package) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;

	int status = processRead(cpu->memoryStruct, pageBytes);

	char* serializedData = serialize_t_PageBytes(pageBytes);

	if (status == 0) {
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_GET_PAGE_BYTES_RESPONSE, package->size,
				serializedData);
				// serialize_t_PageBytes (pageBytes));
		logTrace ("Se devuelven los datos pedidos por CPU para pid: %i pag: %i offset: %i size: %i", pageBytes->pid,
				pageBytes->pageNumber, pageBytes->offset, pageBytes->size);
	}
	else {
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);
		logError ("La peticion de lectura de CPU produce segmentation fault");
	}

	free (serializedData);
	destroyPackage(outPackage);
	destroy_t_PageBytes(pageBytes);
}
