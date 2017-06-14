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

	if (status == 0)
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_SAVE_PAGE_BYTES_RESPONSE, 0, NULL);
	else
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroyPackage(outPackage);
	destroy_t_PageBytes(pageBytes);
}

void cpuReadData(CPU* cpu, Package* package) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;

	int status = processRead(cpu->memoryStruct, pageBytes);


	if (status == 0)
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				COD_GET_PAGE_BYTES_RESPONSE, pageBytes->size,
				serialize_t_PageBytes (pageBytes));
	else
		outPackage = createAndSendPackage(cpu->fileDescriptor,
				ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroy_t_PageBytes(pageBytes);
	destroyPackage(outPackage);
}
