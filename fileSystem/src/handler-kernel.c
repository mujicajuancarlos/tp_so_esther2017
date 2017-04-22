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
		 * 2. Un cliente ya conectado que envía un mensaje
		 * Erratas: el select para que sea más amigable el código, podría romper
		 * tranquilamente en caso de que quiera mandar otro proceso*/
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
	destroyPackage(package);
}

// COMUNICACIÓN C/KERNEL

void comunicacionConKernel(fileSystem_struct *args){

	Package *package;
	//int init = 1;
	//int exit = 1;

	while (1){
		handleKernelRequest(args, package);
	}
	/*while (exit){
		packagesSenderKernel(fileSystem_struct *args, exit);
	}*/
}

void packagesSenderKernel(fileSystem_struct *fileDesc, int exit, int code){
	/*Cuando se le pide al proceso que haga alguna de las operaciones,
	se le puede mandar esta función con el código setteado (Se le podría cambiar
	el nombre a esta función)*/
	exit = 1;
	if(code < 0){
		puts("La especificada, no es una operacion valida");
	} else {
	while (exit){
	 switch(code)	{
		case COD_CREAR_ARCHIVO:
			//	Enviar un mensaje de que fue satisfactorio
			break;
		case COD_BORRAR_ARCHIVO:
			//	Verificar que existe ese archivo del path, borrarlo y mencionar el éxito del proceso
			break;
		case COD_OBTENER_DATOS:
			//	Si pide datos el Kernel, y el path está en modo lectura, se devolverá la cant de bytes definidos por el size en base al offset
			break;
		case COD_GUARDAR_DATOS:
			//	En caso de guardar y que no haya espacio
			exceptionTo(fileDesc,"No es guardar datos, no hay espacio disponible");
			break;
		case COD_SALUDO: //Cuando se conectan, no hacen un handshake ya?
			/*doHandshake(fd_kernel, code);*/
			break;
		}
	}

}
	void exceptionTo(int destinationfd, char *exception){
		int len, bytes_sent;
		len = strlen(exception);
		bytes_sent = sendmsg(destinationfd,exception,len);
		if (bytes_sent != -1){
			puts("Eror en mandar exception");
		}
		else
		{
			puts("Se mando corectamente mensaje de exception");
		}
	}
