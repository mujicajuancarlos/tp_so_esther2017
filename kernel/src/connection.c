/*
 * connection.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */



#include "connection.h"

/**
 * Creo el socket server para cpus
 */
void createCpuServerSocket(kernel_struct* args) {
	args->socketServerCPU = crearSocketServer(args->config->puerto_cpu);
	if (args->socketServerCPU == -1) {
		logError("No se pudo crear el server para cpu's");
		exit(-1);
	}
	logInfo("Server Socket de cpu's esta escuchando");
}

/**
 * Creo el socket server para consolas
 */
void createConsoleServerSocket(kernel_struct* args) {
	args->socketServerConsola = crearSocketServer(args->config->puerto_program);
	if (args->socketServerConsola == -1) {
		logError("No se pudo crear el server para las consolas");
		exit(-1);
	}
	logInfo("Server Socket de consolas esta escuchando");
}

/**
 * Creo el socket client para memoria
 */
void createMemoryClientSocket(kernel_struct* kernel_struct) {

	Package* package;

	logInfo("Estableciendo conexion con memoria");

	kernel_struct->socketClientMemoria = crearSocketCliente(
			kernel_struct->config->ip_memoria,
			kernel_struct->config->puerto_memoria);
	if (kernel_struct->socketClientMemoria == -1)
		logError("No se pudo establecer la conexion con la memoria");
	else {
		logInfo("Conexion establecida a traves del FD %d",
						kernel_struct->socketClientMemoria);
		logInfo("Esperando solicitud de identificación de la memoria");
		package = createAndReceivePackage(kernel_struct->socketClientMemoria);
		if(package != NULL && package->msgCode == COD_GET_IDENTIFICATION){
			destroyPackage(package);
			package = createAndSendPackage(kernel_struct->socketClientMemoria, COD_HANDSHAKE_KERNEL, 0 , NULL);
			if(package != NULL)
				destroyPackage(package);
			else {
				logError("No se pudo completar el handshake con la memoria");
				kernel_struct->socketClientMemoria = -1;
			}

		} else {
			logError("La memoria solicito una accion no permitida.");
			kernel_struct->socketClientMemoria = -1;
		}
	}

}

/**
 * Creo el socket client para file system
 */
void createFileSystemClientSocket(kernel_struct* kernel_struct) {

	logInfo("Creando socket para conectarse al file system");

	kernel_struct->socketClientFileSystem = crearSocketCliente(kernel_struct->config->ip_fs,
			kernel_struct->config->puerto_fs);
	if (kernel_struct->socketClientFileSystem == -1)
		logError("No se pudo establecer la conexion con el file system");
	else
		logInfo("Conexion establecida a traves del FD %d", kernel_struct->socketClientFileSystem);
}
