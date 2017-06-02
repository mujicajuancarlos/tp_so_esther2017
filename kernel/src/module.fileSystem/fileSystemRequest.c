/*
 * fileSystemRequest.c
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#include "fileSystemRequest.h"

bool validateFile(kernel_struct* kernelStruct, t_fileDescriptor* fileDescriptor) {
	Package* package = createAndSendPackage(
			kernelStruct->socketClientFileSystem, COD_VALIDAR_ARCHIVO,
			strlen(fileDescriptor->path), fileDescriptor->path);
	if (package != NULL) {
		destroyPackage(package);
		package = createAndReceivePackage(kernelStruct->socketClientFileSystem);
		if (package != NULL) {
			bool response = deserialize_int(package->stream);
			if (response) {
				logInfo(
						"El Archivo de fileDescriptor %d ha sido validado correctamente y se encuentra en la ruta %s",
						fileDescriptor->fd, fileDescriptor->path);
				destroyPackage(package);
				return true;
			} else
				logInfo(
						"El Archivo de fileDescriptor %d no se encuentra en la ruta %s",
						fileDescriptor->fd, fileDescriptor->path);
		} else
			logInfo("No se pudo recibir los datos del FileSystem");
	} else
		logInfo("No se pudo enviar los datos al FileSystem");

	destroyPackage(package);
	return false;

}




