/*
 * cpuRequest.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "cpuRequest.h"

void cpuSaveData(Package* package, memory_struct* memoryStruct) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;
	int status = processWrite(memoryStruct, pageBytes);

	if (status == 0)
		outPackage = createAndSendPackage(memoryStruct->socketClientKernel,
				COD_SAVE_PAGE_BYTES_RESPONSE, 0, NULL);
	else
		outPackage = createAndSendPackage(memoryStruct->socketClientKernel,
				ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroyPackage(outPackage);
	destroy_t_PageBytes(pageBytes);
}

void cpuReadData(Package* package, memory_struct* memoryStruct) {
	t_PageBytes* pageBytes = deserialize_t_PageBytes(package->stream);
	Package* outPackage;

	int status = processRead(memoryStruct, pageBytes);

	if (status == 0)
		outPackage = createAndSendPackage(memoryStruct->socketClientKernel,
				COD_GET_PAGE_BYTES_RESPONSE, pageBytes->size,
				pageBytes->buffer);
	else
		outPackage = createAndSendPackage(memoryStruct->socketClientKernel,
				ERROR_SEGMENTATION_FAULT, 0, NULL);

	destroy_t_PageBytes(pageBytes);
	destroyPackage(outPackage);
}
