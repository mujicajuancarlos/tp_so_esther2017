/*
 * memoryManager.c
 *
 *  Created on: 19/5/2017
 *      Author: utnso
 */

#include "memoryManager.h"
/**
 * mi objetivo es solicitar y setear el tmaño de pagina
 */
void loadMemoryPageSize(kernel_struct* kernel_struct) {
	Package* package;
	logInfo("Solicitando a la memoria el tamaño de pagina");
	package = createAndSendPackage(kernel_struct->socketClientMemoria, COD_PAGE_SIZE_REQUEST, 0,
			NULL);
	if(package == NULL){
		logError("No se pudo solicitar el tamaño de pagina");
		exit (-1);//todo ver que codigo de error se asiga en base a la tabla de errores
	}
	destroyPackage(package);

	logInfo("Esperando respuesta de la memoria");
	package = createAndReceivePackage(kernel_struct->socketClientMemoria);
	if (package != NULL && package->msgCode == COD_PAGE_SIZE_RESPONSE) {
		logInfo("Seteando el tamaño de pagina");
		kernel_struct->pageSize = deserialize_int(package->stream);
	} else {
		logError("La memoria respondio con una accion no permitida.");
		exit (-1);//todo ver que codigo de error se asiga en base a la tabla de errores
	}

}
