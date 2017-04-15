/*
 * handlerFileSystem.c
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#include "handler-fileSystem.h"

void handleFileSystem(kernel_struct *args) {
	int fileDescriptor;
	logInfo("Creando socket para conectarse al fileSystem");

	fileDescriptor = crearSocketCliente(args->config->ip_fs,
			args->config->puerto_fs);

	if (fileDescriptor == -1)
		logError("No se pudo establecer la conexion con el fileSystem");
	else {
		char* message = "Hola soy el kernel";
		int len = strlen(message);
		if (send(fileDescriptor, message, len, 0) != -1) {
			puts("Se mando mensaje al fileSystem corectamente");
		};
	}
}
