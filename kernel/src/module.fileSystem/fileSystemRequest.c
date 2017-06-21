/*
 * fileSystemRequest.c
 *
 *  Created on: 1/6/2017
 *      Author: utnso
 */

#include "fileSystemRequest.h"

void validateExistFileRequest(Process* process, char* path, int* status) {
	Package* tmpPackage;
	int fsSocket = process->kernelStruct->socketClientFileSystem;
	tmpPackage = createAndSendPackage(fsSocket, COD_EXIST_FILE_REQUEST,
			strlen(path), path);
	if (tmpPackage == NULL) {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo(
			"Se solicitó al FS verificar la existencia del archivo: %s para el pid: %d",
			path, process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(fsSocket);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_FS_RESPONSE_OK:
			logInfo(
					"El FS verifico la existencia del archivo: %s para el pid: %d",
					path, process->pid);
			*status = COD_FS_RESPONSE_OK;
			break;
		case COD_FS_RESPONSE_ERROR:
			logInfo("El FS indico que no existe el archivo: %s", path);
			*status = FILE_NOTFOUND_FD_FAILURE;
			break;
		default:
			logError("El FS respondio con un mensaje desconocido");
			*status = FS_ERROR_UNKNOWN;
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
}

void createFileRequest(Process* process, char* path, int* status) {
	Package* tmpPackage;
	int fsSocket = process->kernelStruct->socketClientFileSystem;
	tmpPackage = createAndSendPackage(fsSocket, COD_CREATE_FILE_REQUEST,
			strlen(path), path);
	if (tmpPackage == NULL) {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo("Se solicitó al FS crear el archivo: %s para el pid: %d", path,
			process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(fsSocket);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_FS_RESPONSE_OK:
			logInfo("El FS creo el archivo: %s", path);
			*status = COD_FS_RESPONSE_OK;
			break;
		case COD_FS_RESPONSE_ERROR:
			logInfo("El FS no pudo crear el archivo: %s", path);
			//todo definir en fs los codigos de error -> ej: no hay espacio,
			*status = FILE_NOTFOUND_FD_FAILURE;
			break;
		default:
			logError("El FS respondio con un mensaje desconocido");
			*status = FS_ERROR_UNKNOWN;
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
}

void deleteFileRequest(Process* process, char* path, int* status) {
	Package* tmpPackage;
	int fsSocket = process->kernelStruct->socketClientFileSystem;
	tmpPackage = createAndSendPackage(fsSocket, COD_DELETE_FILE_REQUEST,
			strlen(path), path);
	if (tmpPackage == NULL) {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo("Se solicitó al FS eliminar el archivo: %s para el pid: %d", path,
			process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(fsSocket);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_FS_RESPONSE_OK:
			logInfo("El FS eliminó el archivo: %s", path);
			*status = COD_FS_RESPONSE_OK;
			break;
		case COD_FS_RESPONSE_ERROR:
			logInfo("El FS no pudo eiminar el archivo: %s", path);
			*status = FILE_NOTFOUND_FD_FAILURE;
			break;
		default:
			logError("El FS respondio con un mensaje desconocido");
			*status = FS_ERROR_UNKNOWN;
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
}

void writeFileRequest(Process* process, t_fileData* data, int* status) {
	Package* tmpPackage;
	int fsSocket = process->kernelStruct->socketClientFileSystem;
	size_t size = sizeof_t_fileData(data);
	char* buffer = serialize_t_fileData(data);
	tmpPackage = createAndSendPackage(fsSocket, COD_WRITE_FILE_REQUEST, size,
			buffer);
	free(buffer);
	if (tmpPackage == NULL) {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo("Se solicitó al FS escribir el archivo: %s para el pid: %d",
			data->path, process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(fsSocket);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_FS_RESPONSE_OK:
			logInfo("El FS escribio en el archivo: %s", data->path);
			*status = COD_FS_RESPONSE_OK;
			break;
		case COD_FS_RESPONSE_ERROR:
			logInfo("El FS no pudo escribir en el archivo: %s", data->path);
			//todo definir en fs los codigos de error -> ej: no hay espacio
			*status = FILE_NOTFOUND_FD_FAILURE;
			break;
		default:
			logError("El FS respondio con un mensaje desconocido");
			*status = FS_ERROR_UNKNOWN;
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
}

void readFileRequest(Process* process, t_fileData* data, int* status) {
	Package* tmpPackage;
	int fsSocket = process->kernelStruct->socketClientFileSystem;
	size_t size = sizeof_t_fileData(data);
	char* buffer = serialize_t_fileData(data);
	free(buffer);
	tmpPackage = createAndSendPackage(fsSocket, COD_READ_FILE_REQUEST, size,
			buffer);
	if (tmpPackage == NULL) {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
	destroyPackage(tmpPackage);
	logInfo("Se solicitó al FS leer el archivo: %s para el pid: %d",
			data->path, process->pid);
	//me quedo a la espera de la aprobacion
	tmpPackage = createAndReceivePackage(fsSocket);
	if (tmpPackage != NULL) {
		switch (tmpPackage->msgCode) {
		case COD_FS_RESPONSE_OK:
			logInfo("El FS leyó el archivo: %s", data->path);
			memcpy(data->data,tmpPackage->stream,tmpPackage->size);
			*status = COD_FS_RESPONSE_OK;
			break;
		case COD_FS_RESPONSE_ERROR:
			logInfo("El FS no pudo escribir en el archivo: %s", data->path);
			//todo definir en fs los codigos de error -> ej: no hay archivo, no se pudo eliminar por otro uso
			*status = FILE_NOTFOUND_FD_FAILURE;
			break;
		default:
			logError("El FS respondio con un mensaje desconocido");
			*status = FS_ERROR_UNKNOWN;
			break;
		}
		destroyPackage(tmpPackage);
	} else {
		logError("El file system no esta conectado");
		exit(EXIT_FAILURE);
	}
}

