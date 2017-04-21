/*
 * handler-kernel.c
 *
 *  Created on: 18/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernel(fileSystem_struct *args) {

	fd_set descriptoresLectura;
	int fd_fileSystem = args->socketServer;
	int fd_kernel = -1;
	int activity;

	while (1) {

		//borra el set de descriptores
		FD_ZERO(&descriptoresLectura);
		//agrega el socket cliente del kernel para recibir instrucciones
		FD_SET(fd_fileSystem, &descriptoresLectura);

		logInfo("Esperando instrucciones del kernel");
		/* Espera indefinida hasta que alguno de los descriptores tenga algo
		 * que decir -> puede ser producido por:
		 * 1. Nuevo cliente solicitando conectarse
		 * 2. Un cliente ya conectado que envía un mensaje*/
		activity = select(fd_fileSystem + 1, &descriptoresLectura, NULL,
		NULL, NULL);
		if ((activity < 0) && (errno != EINTR))
			logError("Error al hacer el select del socket server de cpu");


		if (FD_ISSET(fd_fileSystem, &descriptoresLectura)) {

			if(fd_kernel == -1){
				fd_kernel = aceptarConexionCliente(fd_fileSystem);
				args->fd_kernel = fd_kernel;
			} else
				logError("No es posible conectarse ya existe un proceso conectado");
		}

		if (FD_ISSET(args->fd_kernel, &descriptoresLectura)) {
			Package* package = createEmptyPackage();
			if (receivePackage(args->fd_kernel, package) == 0)
				handleKernelRequest(args, package);
			else
				logError("Error al intentar recibir los datos del FD: %d",
						fd_fileSystem);
			destroyPackage(package);
		}
	}
}


void handleKernelRequest(fileSystem_struct *args, Package *package) {

	switch (package->msgCode) {
		case COD_VALIDAR_ARCHIVO:
			//	Verifica que el archivo del path exista
			break;
		case COD_CREAR_ARCHIVO:
			//	En caso de que no exista (y en el path se pueda escribir) se va a crear el archivo dentro de ese path
			break;
		case COD_BORRAR_ARCHIVO:
			//	En caso de que exista, borrará el archivo metadata y liberará bloques del bitmap
			break;
		case COD_OBTENER_DATOS:
			//	Si pide datos el Kernel, y el path está en modo lectura, se devolverá la cant de bytes definidos por el size en base al offset
			break;
		case COD_GUARDAR_DATOS:
			//	Si se encuentra en modo escritura, se almacenará en el path los bytes del buffer definidos por el size
			break;
		case COD_SALUDO:
			logInfo("El kernel %d me envio el siguiente saludo: %s", args->fd_kernel,
				package->message);
		break;
	default:
		logError("El kernel solicito una accion no permitida");
		break;
	}
}

