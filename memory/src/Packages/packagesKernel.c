/*
 * packagesKernel.c
 *
 *  Created on: 20/4/2017
 *      Author: utnso
 */

#include "packagesKernel.h"

void comunicacionConKernel(int socketKernelfd){

	int init = 1;
	int exit = 1;

	while (init){
		packagesReceptionKernel(socketKernelfd, init);
	}
	while (exit){
		packagesSenderKernel(socketKernelfd, exit);
	}
}

void packagesReceptionKernel(int kernelfd, int continuador){

	Package *packageRecv;

	if(receivePackage(kernelfd, packageRecv) != 0){
	switch(packageRecv->msgCode)
	{
	case COD_ESCRITURA_PAGE:
		//Hace lo que tiene que hacer
		break;
	case COD_ASIGN_PAGE_PROCESS:
		//Hace lo que tiene que hacer
		break;
	case COD_FINALIZAR_PROGRAMA:
		continuador = 0;
		logDebug("El Kernel informa que finalizó el programa");
		break;
	case COD_SALUDO:
		//Hace lo que tenga que hacer

	}
	destroyPackage(packageRecv);
}
}

void packagesSenderKernel(int kernelfd, int exit, int code){ //Cuando se le pide al proceso que haga alguna de las operaciones, se le puede mandar esta función con el código setteado (Se le podría cambiar el nombre a esta función)

	exit = 1;

	if(code < 0){

		puts("La especificada, no es una operacion valida");

	} else {

	while (exit){



		switch(code)
		{
			case COD_ESCRITURA_PAGE:
				//En realidad, no tiene que devolver nada, pero podemos hacer un send a Kernel o CPU diciendo que fue exitoso
				break;
			case COD_ASIGN_PAGE_PROCESS:
				//Informar la imposibilidad de asignar páginas por falta de espacio

				exceptionTo(kernelfd,"No es posible asignar paginas por falta de espacio");
				break;
			case COD_SALUDO: //Cuando se conectan, no hacen un handshake ya?
				doHandshake(kernelfd, code);

		}
	}
	}

}


void doHandshake(int destinationfd, int code){ //puede estar en las dc-commons para no repetir lógica luego

	int status;
	uint32_t size_message = 200;
	char* message = malloc(sizeof(char) * size_message);

	printf("\nIngrese un mensaje:\n");
	fgets(message, size_message, stdin);

	if (strcmp(message, "") == 0 || strcmp(message, "exit") == 0){
			exit = 1;
	} else {
			Package *package = createPackage(code, message, size_message);
			status = sendPackage(destinationfd, package);
			if (status != -1)
				printf("\nEnvio OK\n");
			destroyPackage(package);
		}
		free(message);
}

void exceptionTo(int destinationfd, char *exception){

	int len, bytes_sent;
	len = strlen(exception);

	bytes_sent = send(destinationfd, exception, len, 0); //Verificar

}
