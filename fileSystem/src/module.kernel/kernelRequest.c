/*
 * kernelRequest.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "kernelRequest.h"

void executeExistFileRequest(fileSystem_struct* fsStruct, Package* package) {
	Package* tmpPackage = NULL;
	char* path = string_substring_until(package->stream, package->size);
	logInfo("El kernel solicito verificar la existencia del archivo %s",path);
	bool exist = basicExistFile(fsStruct, path);
	if (exist) {
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
				COD_FS_RESPONSE_OK, 0, NULL);
		logInfo("Se informo que el archivo %s existe en el file system %s",path);
	} else {
		tmpPackage = createAndSendPackage(COD_FS_RESPONSE_ERROR,
				COD_FS_RESPONSE_ERROR, 0, NULL);
		logInfo("Se informo que el archivo %s NO existe en el file system %s",path);
	}
	free(path);
	destroyPackage(tmpPackage);
}

void executeCreateFileRequest(fileSystem_struct* fsStruct, Package* package) {
	Package* tmpPackage = NULL;
	char* path = string_substring_until(package->stream, package->size);
	int status;
	logInfo("El kernel solicito verificar la existencia del archivo %s",path);
	basicCreateFile(fsStruct, path, &status);
	switch (status) {
		case EXC_OK:
			tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
					COD_FS_RESPONSE_OK, 0, NULL);
			break;
		case EXC_ERROR_EXIST_FILE:
			tmpPackage = createAndSendPackage(COD_FS_RESPONSE_ERROR,
							COD_FS_RESPONSE_ERROR, 0, NULL);
			break;
	}
	free(path);
	destroyPackage(tmpPackage);
}
