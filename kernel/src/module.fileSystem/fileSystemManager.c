/*
 * fileSystemManager.c
 *
 *  Created on: 17/6/2017
 *      Author: utnso
 */

#include "fileSystemManager.h"

int basicOpenProcessFile(Process* process, t_new_FD_request* dataRequest,
		int* assignedFD) {
	int status = OPEN_FD_SUCCESS;
	t_globalFile* globalFile = getGlobalFileFor(dataRequest->path);
	if (globalFile == NULL) {
		globalFile = createGlobalFileWith(dataRequest, &status);
		if (globalFile == NULL)
			return PERMISSIONS_DENIED_FD_FAILURE;
	}
	*assignedFD = createProcessFileWith(process, globalFile,
			dataRequest->flags);
	//todo verificar si es necesario enviar a fs
	return status;
}

int basicDeleteProcessFile(Process* process, int fileDescriptor) {
	int status = DELETE_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, fileDescriptor);
	validateExistFile(file, &status);
	if (*status == VALIDATION_FD_OK) {
		removeAndDestroyFile(file);
		//todo enviar la eliminacion del archivo al fs
	}
	return status;
}

int basicCloseProcessFile(Process* process, int fileDescriptor) {
	int status = CLOSE_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, fileDescriptor);
	validateExistFile(file, &status);
	if (*status == VALIDATION_FD_OK) {
		removeAndDestroyFile(file);
		//todo verificar si es necesario enviar a fs
	}
	return status;
}

int basicSeekProcessFile(Process* process, t_seed_FD_request* dataRequest) {
	int status = SEEK_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, dataRequest->fd);
	validateExistFile(file, &status);
	if (*status == VALIDATION_FD_OK) {
		file->seekValue = dataRequest->offset;
	}
	return status;
}

int basicWriteProcessFile(Process* process, t_data_FD_request* dataRequest) {
	int status = WRITE_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, dataRequest->fd);
	validateExistFile(file, &status);
	if (*status == VALIDATION_FD_OK) {
		validatePermissionForWriteFile(file, &status);
		if (*status == VALIDATION_FD_OK) {
			//todo enviar la solicitud a fs
		}
	}
	return status;
}

int basicReadProcessFile(Process* process, t_dataPointer_FD_request dataRequest) {
	int status = READ_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, dataRequest->fd);
	validateExistFile(file, &status);
	if (*status == VALIDATION_FD_OK) {
		validatePermissionForReadFile(file, &status);
		if (*status == VALIDATION_FD_OK) {
			//todo enviar la solicitud a fs
		}
	}
	return status;
}

/*
 * previo se debe asegurar de tener un global file valido
 */
int createProcessFileWith(Process* process, t_globalFile* globalFile,
		t_banderas externalFlags) {
	int fileDescriptor = getNextFileDescriptorFor(process);
	t_flags internalFlags;
	internalFlags.read = externalFlags.lectura;
	internalFlags.write = externalFlags.escritura;
	logInfo("Creando fd #%d de proceso para el archivo %s", fileDescriptor,
			globalFile->path);
	t_processFile* processFile = create_t_processFile(fileDescriptor,
			globalFile, internalFlags);
	addFile(process, processFile);
	return fileDescriptor;
}
/*
 * invocar solo cuando se verifico que el archivo no existe
 */
t_globalFile* createGlobalFileWith(t_new_FD_request* dataRequest, int* status) {
	t_globalFile* globalFile = NULL;
	validatePermissionForCreateFile(dataRequest->flags, status);
	if (*status == VALIDATION_FD_OK) {
		logInfo("Creando fd global para el archivo %s", dataRequest->path);
		globalFile = create_t_globalFile(dataRequest->path);
		addGlobalFile(globalFile);
	}
	return globalFile;
}

void validatePermissionForCreateFile(t_banderas flags, int* status) {
	*status = flags.creacion ? VALIDATION_FD_OK : PERMISSIONS_DENIED_FD_FAILURE;
}

void validateExistFile(t_processFile* file, int* status) {
	*status = (file != NULL) ? VALIDATION_FD_OK : FILE_NOTFOUND_FD_FAILURE;
}

void validatePermissionForWriteFile(t_processFile* file, int* status) {
	*status =
			file->flag.write ? VALIDATION_FD_OK : PERMISSIONS_DENIED_FD_FAILURE;
}

void validatePermissionForReadFile(t_processFile* file, int* status) {
	*status =
			file->flag.read ? VALIDATION_FD_OK : PERMISSIONS_DENIED_FD_FAILURE;
}
