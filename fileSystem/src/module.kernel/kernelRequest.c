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
	bool exist = basicExistFile(fsStruct, path);
	logInfo("El kernel solicito verificar la existencia del archivo %s",path);
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

/*
 * existFile(Path);
 *
 * createFile(Path);
 *
 * deleteFile(Path);
 *
 * readFile(Data);
 *
 * writeFile(Data);
 */
