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
	char* path = string_substring_until(dataRequest->path,
			dataRequest->sizePath);
	t_globalFile* globalFile = getGlobalFileFor(path);
	if (globalFile == NULL) {
		validateExistFileRequest(process, path, &status);
		if (status == FILE_NOTFOUND_FD_FAILURE) {
			logInfo("El archivo %s no existe en FS", path);
			logInfo("Validando permisos", path);
			validatePermissionForCreateFile(dataRequest->flags, &status);
			if (status == VALIDATION_FD_OK) {
				createFileRequest(process, path, &status);
				if (status == COD_FS_RESPONSE_OK) {
					logInfo(
							"El FS indico que el archivo %s se creo sin problemas");
					globalFile = createGlobalFileWith(dataRequest);
					status = OPEN_FD_SUCCESS;
				} else {
					free(path);
					return WITHOUT_RESOURCES_FD_FAILURE;
				}
			} else {
				logInfo(
						"El archivo %s no tiene permisos de creacion, la solicitud no puede continuar");
				free(path);
				return PERMISSIONS_DENIED_FD_FAILURE;
			}
		} else {
			globalFile = createGlobalFileWith(dataRequest);
		}
	}
	free(path);
	*assignedFD = createProcessFileWith(process, globalFile,
			dataRequest->flags);
	status = OPEN_FD_SUCCESS;
	return status;
}

int basicDeleteProcessFile(Process* process, int fileDescriptor) {
	int status = DELETE_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, fileDescriptor);
	validateExistFile(file, &status);
	if (status == VALIDATION_FD_OK) {
		validateDeleteFile(file->globalFile, &status);
		if (status == VALIDATION_FD_OK) {
			removeAndDestroyFile(process, file);
			deleteFileRequest(process, file->globalFile->path, &status);
			if (status == COD_FS_RESPONSE_OK) {
				status = DELETE_FD_SUCCESS;
			} else {
				status = FILE_NOTFOUND_FD_FAILURE;
			}
		} else {
			status = FILE_IN_USED_FD_FAILUERE;
		}
	}
	return status;
}

int basicCloseProcessFile(Process* process, int fileDescriptor) {
	int status = CLOSE_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, fileDescriptor);
	validateExistFile(file, &status);
	if (status == VALIDATION_FD_OK) {
		removeAndDestroyFile(process, file);
		status = CLOSE_FD_SUCCESS; //no es necesario informar a fs
	}
	return status;
}

int basicSeekProcessFile(Process* process, t_seed_FD_request* dataRequest) {
	int status = SEEK_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, dataRequest->fd);
	validateExistFile(file, &status);
	if (status == VALIDATION_FD_OK) {
		file->seekValue = dataRequest->offset;
		status = SEEK_FD_SUCCESS; //no es necesario informa a fs
	}
	return status;
}

int basicWriteProcessFile(Process* process, t_data_FD_request* dataRequest) {
	int status = WRITE_FD_SUCCESS;
	if (dataRequest->fd == STDOUT_FD) {
		writeOnStdout(process, dataRequest);
		return WRITE_FD_SUCCESS;
	}
	t_processFile* file = getProcessFile(process, dataRequest->fd);
	validateExistFile(file, &status);
	if (status == VALIDATION_FD_OK) {
		validatePermissionForWriteFile(file, &status);
		if (status == VALIDATION_FD_OK) {
			t_fileData* fileData = create_t_fileData(file->globalFile->path,
					file->seekValue, dataRequest->sizeBuffer);
			memcpy(fileData->data, dataRequest->buffer,
					dataRequest->sizeBuffer);
			writeFileRequest(process, fileData, &status);
			if (status == COD_FS_RESPONSE_OK) {
				status = WRITE_FD_SUCCESS;
			} else {
				status = FILE_NOTFOUND_FD_FAILURE;
			}
			destroy_t_fileData(fileData);
		}
	}
	return status;
}

int basicReadProcessFile(Process* process,
		t_dataPointer_FD_request* dataRequest) {
	int status = READ_FD_SUCCESS;
	t_processFile* file = getProcessFile(process, dataRequest->fd);
	validateExistFile(file, &status);
	if (status == VALIDATION_FD_OK) {
		validatePermissionForReadFile(file, &status);
		if (status == VALIDATION_FD_OK) {
			t_fileData* fileData = create_t_fileData(file->globalFile->path,
					file->seekValue, dataRequest->size);
			readFileRequest(process, fileData, &status);
			if (status == COD_FS_RESPONSE_OK) {
				dir_memoria address = pointerToMemoryLogicalAddress(
						dataRequest->pointer, process);
				bool hasError;
				logInfo(
						"Enviando la informacion del archivo leido a la memoria");
				saveDataOnMemory(process, address.pagina, address.offset,
						fileData->dataSize, fileData->data, &hasError);
				if (!hasError) {
					status = READ_FD_SUCCESS;
				} else {
					status = MEMORY_SAVE_FAILURE;
				}
			} else {
				if(status != FS_FILE_BLOCK_FAULT){
					status = FILE_NOTFOUND_FD_FAILURE;
				}
			}
			destroy_t_fileData(fileData);
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
t_globalFile* createGlobalFileWith(t_new_FD_request* dataRequest) {
	t_globalFile* globalFile = NULL;
	char* path = string_substring_until(dataRequest->path,
			dataRequest->sizePath);
	logInfo("Creando fd global para el archivo %s", path);
	globalFile = create_t_globalFile(path);
	addGlobalFile(globalFile);

	free(path);
	return globalFile;
}

void validateDeleteFile(t_globalFile* globalFile, int* status) {
	*status = (globalFile->open == 1) ?
	VALIDATION_FD_OK :
										FILE_IN_USED_FD_FAILUERE;
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
