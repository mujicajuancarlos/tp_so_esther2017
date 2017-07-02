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
	while (true) {

		logInfo("Esperando nueva conexion del kernel");
		fsStruct->fd_kernel = aceptarConexionCliente(fsStruct->socketServer);

		if (fsStruct->fd_kernel != -1) {
			logInfo("El kernel se conectó exitosamente");

			package = createAndReceivePackage(fsStruct->fd_kernel);

			/**
			 * Mientras mantenga la conexion atiendo los pedidos
			 */
			while (package != NULL) {
				handleKernelRequest(fsStruct, package);
				package = createAndReceivePackage(fsStruct->fd_kernel);
			}

			logError("El kernel cerro la conexion para FD: %d",
					fsStruct->fd_kernel);
		} else {
			logError("El kernel no se pudo conectar");
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
