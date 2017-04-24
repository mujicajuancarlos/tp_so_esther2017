/*
 * handler-kernel.c
 *
 *  Created on: 15/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernel(cpu_struct *args) {

	fd_set descriptoresLectura;
	int fd_kernel = args->socketClientKernel;
	int activity;

	while (1) {

		//borra el set de descriptiores
		FD_ZERO(&descriptoresLectura);
		//agrega el socket cliente del kernel para recibir instrucciones
		FD_SET(fd_kernel, &descriptoresLectura);

		logInfo("Esperando instrucciones del kernel");
		/* Espera indefinida hasta que alguno de los descriptores tenga algo
		 * que decir -> puede ser producido por:
		 * 1. Nuevo cliente solicitando conectarse
		 * 2. Un cliente ya conectado que envía un mensaje*/
		activity = select(fd_kernel + 1, &descriptoresLectura, NULL,
		NULL, NULL);
		if ((activity < 0) && (errno != EINTR))
			logError("Error al hacer el select del socket server de cpu");

		if (FD_ISSET(fd_kernel, &descriptoresLectura)) {
			Package* package = createEmptyPackage();
			if (receivePackage(fd_kernel, package) == 0)
				handleKernelRequest(args, package);
			else
				logError("Error al intentar recibir los datos del FD: %d",
						fd_kernel);
			destroyPackage(package);
		}
	}
}

void handleKernelRequest(cpu_struct *args, Package *package) {
	int fd_kernel = args->socketClientKernel;
	int fd_memoria = args->socketClientMemoria;

	switch (package->msgCode) {
	case COD_SALUDO:
		logInfo("El kernel %d me envio el siguiente saludo: %s", fd_kernel,
				package->stream);
		if (sendPackage(fd_memoria, package) == -1)
			logError("No se pudo enviar el saludo a la memoria fd: %d",
					fd_memoria);
		else
			logInfo("Se envio la informacion a la memoria fd: %d", fd_memoria);
		break;
	default:
		logError("El kernel solicito una accion no permitida");
		break;
	}
}
