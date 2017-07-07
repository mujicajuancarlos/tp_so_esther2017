/*
 * handler-kernel.c
 *
 *  Created on: 18/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

/**
 * Estoy configurado para atender una sola conexion a la vez (Un unico kernel)
 */
void handleKernel(fileSystem_struct* fsStruct) {

	Package* package;
	bool running;
	while (true) {

		logInfo("Esperando una conexion del kernel");
		fsStruct->fd_kernel = aceptarConexionCliente(fsStruct->socketServer);

		if (fsStruct->fd_kernel != -1) {
			logInfo("El kernel se conectó exitosamente a travez del fd %d",
					fsStruct->fd_kernel);
			/**
			 * Mientras mantenga la conexion atiendo los pedidos
			 */
			running = true;
			while (running) {
				logInfo("Esperando una solicitud del kernel");
				package = createAndReceivePackage(fsStruct->fd_kernel);
				if (package != NULL) {
					handleKernelRequest(fsStruct, package);
				} else {
					running = false;
					close(fsStruct->fd_kernel);
					logInfo("El kernel cerro la conexion del FD: %d",
										fsStruct->fd_kernel);
				}
				destroyPackage(package);
			}
		} else {
			logError("El kernel no se pudo conectar,reintente nuevamente");
		}
	}
}

void handleKernelRequest(fileSystem_struct* fsStruct, Package* package) {

	switch (package->msgCode) {
	case COD_EXIST_FILE_REQUEST:
		executeExistFileRequest(fsStruct, package);
		break;
	case COD_CREATE_FILE_REQUEST:
		executeCreateFileRequest(fsStruct, package);
		break;
	case COD_DELETE_FILE_REQUEST:
		executeDeleteFileRequest(fsStruct, package);
		break;
	case COD_READ_FILE_REQUEST:
		executeReadFileRequest(fsStruct, package);
		break;
	case COD_WRITE_FILE_REQUEST:
		executeWriteFileRequest(fsStruct, package);
		break;
	default:
		logError("El kernel solicitó una accion no permitida");
		break;
	}

	destroyPackage(package);
}
