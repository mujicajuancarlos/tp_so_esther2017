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
void handleKernel(fileSystem_struct *fsStruct) {

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

void handleKernelRequest(fileSystem_struct *fsStruct, Package *package) {

	/**
	 * TODO verificar los codigos de mensaje permitidos entre kernel y fs
	 */
	switch (package->msgCode) {
	case COD_EXIST_FILE_REQUEST:
		//	Verifica que el archivo del path exista
		break;
	case COD_CREATE_FILE_REQUEST:
		//	En caso de que no exista (y en el path se pueda escribir) se va a crear el archivo dentro de ese path
		break;
	case COD_DELETE_FILE_REQUEST:
		//	En caso de que exista, borrará el archivo metadata y liberará bloques del bitmap
		break;
	case COD_READ_FILE_REQUEST:
		//	Si pide datos el Kernel, y el path está en modo lectura, se devolverá la cant de bytes definidos por el size en base al offset
		break;
	case COD_WRITE_FILE_REQUEST:
		//	Si se encuentra en modo escritura, se almacenará en el path los bytes del buffer definidos por el size
		break;
	default:
		logError("El kernel solicitó una accion no permitida");
		break;
	}

	destroyPackage(package);
}
