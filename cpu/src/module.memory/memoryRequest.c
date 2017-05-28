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

char* getDataFromPage(cpu_struct* cpuStruct, int pageNumber, int offset,
		int size) {
	Package* package;
	size_t bufferSize = sizeof(char) * size;
	char* buffer = malloc(bufferSize);
	t_PageBytes* data = create_t_PageBytes(getPCB()->pid, pageNumber, offset,
			size, buffer);
	char* serializedData = serialize_t_PageBytes(data);
	size_t serializedSize = sizeof_t_PageBytes(data);
	logTrace("Solicitando datos para pid: %d pag: %d offset: %d size: %d",
			getPCB()->pid, pageNumber, offset, size);
	package = createAndSendPackage(cpuStruct->socketClientMemoria,
	COD_GET_PAGE_BYTES_REQUEST, serializedSize, serializedData);
	free(serializedData);
	destroy_t_PageBytes(data);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(cpuStruct->socketClientMemoria);
		if (package != NULL) {
			switch (package->msgCode) {
			case COD_GET_PAGE_BYTES_RESPONSE:
				data = deserialize_t_PageBytes(package->stream);
				memcpy(buffer,data->buffer,bufferSize);
				destroy_t_PageBytes(data);
				break;
			case ERROR_SEGMENTATION_FAULT:
				logError(
						"La memoria informo que la solicitud produjo SegmentationFault");
				setErrorFlag(FLAG_SEGMENTATION_FAULT);
				break;
			default:
				logError("La memoria respondio con un codigo no esperado");
				setErrorFlag(FLAG_UNKNOWN_ERROR);
				break;
			}
			destroyPackage(package);
		} else {
			logError("No se pudo recibir la solicitud a la memoria");
			setErrorFlag(FLAG_DISCONNECTED_MEMORY);
		}

	} else {
		logError("No se pudo enviar la solicitud a la memoria");
		setErrorFlag(FLAG_DISCONNECTED_MEMORY);
	}
	if(getErrorFlag()!=FLAG_OK){
		free(buffer);
		buffer = NULL;
	}
	return buffer;
}
