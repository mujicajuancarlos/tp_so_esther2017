/*
 * fileSystemManager.c
 *
 *  Created on: 17/6/2017
 *      Author: utnso
 */


#include "fileSystemManager.h"

int basicOpenProcessFile(Process* process, t_new_FD_request* dataRequest,int* assignedFD){
	int status = OPEN_FD_SUCCESS;
	t_globalFile* globalFile = getGlobalFileFor(dataRequest->path);
	if(globalFile == NULL){
		globalFile = createGlobalFileWith(dataRequest, &status);
		if(globalFile == NULL) return PERMISSIONS_DENIED_FD_FAILURE;
	}
	*assignedFD = createProcessFileWith(process,globalFile,dataRequest->flags);
	return status;
}

int basicDeleteProcessFile(Process* process, int fileDescriptor){

	return DELETE_FD_SUCCESS;
}

int basicCloseProcessFile(Process* process, int fileDescriptor){

	return CLOSE_FD_SUCCESS;
}

int basicSeekProcessFile(Process* process, t_seed_FD_request* dataRequest){

	return SEEK_FD_SUCCESS;
}

int basicWriteProcessFile(Process* process, t_data_FD_request* dataRequest){

	return WRITE_FD_SUCCESS;
}

int basicReadProcessFile(Process* process, t_dataPointer_FD_request dataRequest){

	return READ_FD_SUCCESS;
}

/*
 * previo se debe asegurar de tener un global file valido
 */
int createProcessFileWith(Process* process, t_globalFile* globalFile, t_banderas externalFlags){
	int fileDescriptor = getNextFileDescriptorFor(process);
	t_flags internalFlags;
	internalFlags.read = externalFlags.lectura;
	internalFlags.write = externalFlags.escritura;
	logInfo("Creando fd #%d de proceso para el archivo %s", fileDescriptor, globalFile->path);
	t_processFile* processFile = create_t_processFile(fileDescriptor,globalFile,internalFlags);
	addFile(process,processFile);
	return fileDescriptor;
}
/*
 * invocar solo cuando se verifico que el archivo no existe
 */
t_globalFile* createGlobalFileWith(t_new_FD_request* dataRequest, int* status){
	t_globalFile* globalFile = NULL;
	validatePermissionForCreateFile(dataRequest->flags, status);
	if(*status == VALIDATION_FD_OK){
		logInfo("Creando fd global para el archivo %s",dataRequest->path);
		globalFile = create_t_globalFile(dataRequest->path);
		addGlobalFile(globalFile);
	}
	return globalFile;
}

void validatePermissionForCreateFile(t_banderas flags, int* status){
	*status = flags.creacion? VALIDATION_FD_OK : PERMISSIONS_DENIED_FD_FAILURE;
}
