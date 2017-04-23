/*
 * handlerMemory.c
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#include "handler-memory.h"
#include <dc-commons/protocol-memory-kernel.h>

void handleMemoria(kernel_struct *args) {
	int fileDescripor;
	logInfo("Creando socket para conectarse a la memoria");

	fileDescripor = crearSocketCliente(args->config->ip_memoria,
			args->config->puerto_memoria);

	if (fileDescripor == -1)
		logError("No se pudo establecer la conexion con la memoria");
	else {
		char* message = "kernel";
		int len = strlen(message);
		if (send(fileDescripor, message, len, 0) != -1) {
			puts("Se mando mensaje a memoria corectamente");
			args->socketClientMemoria = fileDescripor;

	Package *paqueteParaRecibir = createEmptyPackage();
	int consultaReciv = receivePackage(fileDescripor,paqueteParaRecibir);

		if(consultaReciv != -1){
			printf("Recibido Corectamente el mensaje del paquete es %s\n",paqueteParaRecibir->message);
		}
		else{
			puts("No se pudo recibir");
		}

		};
	}
}

void packagesSenderMemory(int kernelfd, int code){

	if(code < 0){

		puts("La especificada, no es una operacion valida");

	}
	else {

		switch(code)
		{
			case COD_ESCRITURA_PAGE:
				//En realidad, no tiene que devolver nada, pero podemos hacer un send a Kernel o CPU diciendo que fue exitoso
				break;
			case COD_ASIGN_PAGE_PROCESS:
				//Informar la imposibilidad de asignar páginas por falta de espacio

				break;
			case COD_SALUDO:

				break;

		}

	}

}


void packagesReceptionKernel(int memoryFd){

	Package* package = createEmptyPackage();

	if(receivePackage(memoryFd, package) != 0){
	switch(package->msgCode)
	{
	case COD_ESCRITURA_PAGE:
		//Hace lo que tiene que hacer
		break;
	case COD_ASIGN_PAGE_PROCESS:
		//Hace lo que tiene que hacer
		break;
	case COD_FINALIZAR_PROGRAMA:
		logDebug("El Kernel informa que finalizó el programa");
		break;
	case COD_SALUDO:

				printf("Recibido Corectamente el mensaje del paquete es %s\n",package->message);


		break;
	}
	destroyPackage(package);
}
}

