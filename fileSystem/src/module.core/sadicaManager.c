/*
 * sadicaManager.c
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#include "sadicaManager.h"

bool basicExistFile(fileSystem_struct* fsStruct, char* path) {
	return getSadicaFile(fsStruct, path) != NULL;
}

void basicDeleteFile(fileSystem_struct* fsStruct, char* path, int* status) {
	char* fullPath = getFullFilePath(fsStruct, path);
	sadica_file* file = getSadicaFile(fsStruct, fullPath);
	if (file != NULL) {
		removeSadicaFile(fsStruct, file);
		*status = EXC_OK;
	} else {
		*status = EXC_ERROR_FILE_NOT_FOUND;
	}
	free(fullPath);
}

void basicCreateFile(fileSystem_struct* fsStruct, char* path, int*status) {
	char* fullPath = getFullFilePath(fsStruct, path);
	sadica_file* file = getSadicaFile(fsStruct, fullPath);
	if (file == NULL) {
		addSadicaFile(fsStruct, fullPath, status);
	} else {
		*status = EXC_ERROR_EXIST_FILE;
	}
	free(fullPath);
}

void basicWriteFile(fileSystem_struct* fsStruct, char* path, int offset,
		int size, char* buffer, int*status) {
	char* fullPath = getFullFilePath(fsStruct, path);
	sadica_file* file = getSadicaFile(fsStruct, fullPath);
	if (file != NULL) {
		validateBlocksQuantity(file, offset + size, status);
		if (*status == EXC_ERROR_BLOCK_FAULT) {
			resizeBlocksQuantity(fsStruct, file, offset + size, status);
		}
		if (*status == EXC_OK) {
			saveDataOnFS(fsStruct, file, offset, size, buffer);
		}
	} else {
		*status = EXC_ERROR_FILE_NOT_FOUND;
	}
	free(fullPath);
}

void basicReadFile(fileSystem_struct* fsStruct, char* path, int offset,
		int size, char* buffer, int*status) {
	char* fullPath = getFullFilePath(fsStruct, path);
	sadica_file* file = getSadicaFile(fsStruct, fullPath);
	if (file != NULL) {
		validateBlocksQuantity(file, offset + size, status);
		if (*status == EXC_OK) {
			readDataOnFS(fsStruct, file, offset, size, buffer);
		}
	} else {
		*status = EXC_ERROR_FILE_NOT_FOUND;
	}
	free(fullPath);
}

void saveDataOnFS(fileSystem_struct* fsStruct,sadica_file* file,int offset,
		int size, char* buffer){

}

void readDataOnFS(fileSystem_struct* fsStruct,sadica_file* file,int offset,
		int size, char* buffer){

}

void validateBlocksQuantity(sadica_file* file, int maxSize, int*status) {
	*status = (file->size >= maxSize) ? EXC_OK : EXC_ERROR_BLOCK_FAULT;
}

void resizeBlocksQuantity(fileSystem_struct* fsStruct, sadica_file* file,
		int maxSize, int* status) {
	int current = getBlocksQuantity(fsStruct, file->size);
	int new = getBlocksQuantity(fsStruct, maxSize);
	int missing = new - current;
	int* blocks = malloc(sizeof(uint32_t) * missing);
	assignBlocks(fsStruct, missing, blocks, status);
}
