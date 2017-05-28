/*
 * kernelResponse.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "kernelResponse.h"

void reportEndInstruction(cpu_struct* cpuStruct) {
	logInfo("Informando al kernel que la instruccion se ejecutó correctamente");
	Package* package = createAndSendPackage(cpuStruct->socketClientKernel,
			COD_END_INSTRUCCION, 0, NULL);
	destroyPackage(package);
}

void reportExcecutionError(cpu_struct* cpuStruct, int errorFlag) {
	Package* package;
	switch (errorFlag) {
	case FLAG_STACKOVERFLOW:
	case FLAG_SEGMENTATION_FAULT:
	case FLAG_DISCONNECTED_MEMORY:
		package = createAndSendPackage(cpuStruct->socketClientKernel, SC_ERROR_MEMORY_EXCEPTION, 0, NULL);
			break;
	case FLAG_UNKNOWN_ERROR:
		package = createAndSendPackage(cpuStruct->socketClientKernel, SC_ERROR_UNKNOWN, 0, NULL);
			break;
	default:
		package = createAndSendPackage(cpuStruct->socketClientKernel, SC_ERROR_UNKNOWN, 0, NULL);
		break;
	}
	destroyPackage(package);
}
