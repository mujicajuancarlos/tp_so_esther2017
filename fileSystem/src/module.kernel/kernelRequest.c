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
	logInfo("El kernel solicito verificar la existencia del archivo %s", path);
	bool exist = basicExistFile(fsStruct, path);
	if (exist) {
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_OK, 0, NULL);
		logInfo("Se informo que el archivo %s existe en el file system",
				path);
	} else {
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FILE_NOTFOUND, 0, NULL);
		logInfo("Se informo que el archivo %s NO existe en el file system",
				path);
	}
	free(path);
	destroyPackage(tmpPackage);
}

void executeCreateFileRequest(fileSystem_struct* fsStruct, Package* package) {
	Package* tmpPackage = NULL;
	char* path = string_substring_until(package->stream, package->size);
	int status;
	logInfo("El kernel solicito crear del archivo %s", path);
	basicCreateFile(fsStruct, path, &status);
	switch (status) {
	case EXC_OK:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_OK, 0, NULL);
		logInfo("El file system indico crear el archivo %s fue creado", path);
		break;
	case EXC_ERROR_EXIST_FILE:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FILE_EXIST, 0, NULL);
		logInfo("El file system indico al kernel que el archivo %s ya existe",
				path);
		break;
	case EXC_ERROR_FILE_SYSTEM_FULL:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FS_FULL, 0, NULL);
		logInfo(
				"El file system indico al kernel que el archivo %s no se pudo crear por falta de espacio",
				path);
		break;
	}
	free(path);
	destroyPackage(tmpPackage);
}

void executeDeleteFileRequest(fileSystem_struct* fsStruct, Package* package) {
	Package* tmpPackage = NULL;
	char* path = string_substring_until(package->stream, package->size);
	int status;
	logInfo("El kernel solicito eliminar el archivo %s", path);
	basicDeleteFile(fsStruct, path, &status);
	switch (status) {
	case EXC_OK:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_OK, 0, NULL);
		logInfo(
				"El file system indico al kernel que el archivo %s fue eliminado",
				path);
		break;
	case EXC_ERROR_FILE_NOT_FOUND:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FILE_NOTFOUND, 0, NULL);
		logInfo("El file system indico al kernel que el archivo %s no existe",
				path);
		break;
	}
	free(path);
	destroyPackage(tmpPackage);
}

void executeReadFileRequest(fileSystem_struct* fsStruct, Package* package) {
	Package* tmpPackage = NULL;
	t_fileData* data = deserialize_t_fileData(package->stream);
	int status;
	logInfo("El kernel solicito leer del archivo %s", data->path);
	basicReadFile(fsStruct, data->path, data->offset, data->dataSize,
			data->data, &status);
	switch (status) {
	case EXC_OK:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_OK, data->dataSize, data->data);
		logInfo("El file system indico al kernel que el archivo %s fue leido",
				data->path);
		break;
	case EXC_ERROR_FILE_NOT_FOUND:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FILE_NOTFOUND, 0, NULL);
		logInfo("El file system indico al kernel que el archivo %s no existe",
				data->path);
		break;
	}
	destroy_t_fileData(data);
	destroyPackage(tmpPackage);
}

void executeWriteFileRequest(fileSystem_struct* fsStruct, Package* package) {
	Package* tmpPackage = NULL;
	t_fileData* data = deserialize_t_fileData(package->stream);
	int status;
	logInfo("El kernel solicito escribir en el archivo %s", data->path);
	basicWriteFile(fsStruct, data->path, data->offset, data->dataSize,
			data->data, &status);
	switch (status) {
	case EXC_OK:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_OK, 0, NULL);
		logInfo("El file system indico al kernel que el archivo %s fue escrito",
				data->path);
		break;
	case EXC_ERROR_FILE_NOT_FOUND:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FILE_NOTFOUND, 0, NULL);
		logInfo("El file system indico al kernel que el archivo %s no existe",
				data->path);
		break;
	case EXC_ERROR_FILE_SYSTEM_FULL:
		tmpPackage = createAndSendPackage(fsStruct->fd_kernel,
		COD_FS_RESPONSE_FS_FULL, 0, NULL);
		logInfo(
				"El file system indico al kernel que el archivo %s no se pudo crear por falta de espacio",
				data->path);
		break;
	}
	destroy_t_fileData(data);
	destroyPackage(tmpPackage);
}
