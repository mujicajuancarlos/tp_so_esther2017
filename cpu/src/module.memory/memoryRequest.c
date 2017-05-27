/*
 * memoryRequest.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "memoryRequest.h"

/**
 * mi objetivo es solicitar y setear el tmaño de pagina
 */
void loadMemoryPageSize(cpu_struct* cpuStruct) {
	Package* package;
	logInfo("Solicitando a la memoria el tamaño de pagina");
	package = createAndSendPackage(cpuStruct->socketClientMemoria,
			COD_PAGE_SIZE_REQUEST, 0,
	NULL);
	if (package == NULL) {
		logError("No se pudo solicitar el tamaño de pagina");
		exit(ERROR_DISCONNECTED_SOCKET);
	}
	destroyPackage(package);

	logInfo("Esperando respuesta de la memoria");
	package = createAndReceivePackage(cpuStruct->socketClientMemoria);
	if (package != NULL && package->msgCode == COD_PAGE_SIZE_RESPONSE) {
		logInfo("Seteando el tamaño de pagina");
		setPageSize(deserialize_int(package->stream));
	} else {
		logError("La memoria respondio con una accion no permitida.");
		exit(ERROR_UNKNOWN_MESSAGE_CODE);
	}
}

char* getDataFromPage(cpu_struct* cpuStruct, int pageNumber, int offset, int size){
	char* buffer = "devulvo los bytes del buffer";
	//Package* package = createAndSendPackage(cpuStruct->socketClientMemoria, COD_GET_PAGE_BYTES_REQUEST, )
	return buffer;
}
