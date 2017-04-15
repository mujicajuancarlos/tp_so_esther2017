/*
 * handlerMemory.c
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#include "handler-memory.h"

void handleMemoria(kernel_struct *args) {
	int fileDescripor;
	logInfo("Creando socket para conectarse a la memoria");

	fileDescripor = crearSocketCliente(args->config->ip_memoria,
			args->config->puerto_memoria);

	if (fileDescripor == -1)
		logError("No se pudo establecer la conexion con la memoria");
	else {
		char* message = "hola soy el kernel";
		int len = strlen(message);
		if (send(fileDescripor, message, len, 0) != -1) {
			puts("Se mando mensaje a memoria corectamente");
		};
	}
}
