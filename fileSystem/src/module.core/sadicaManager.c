/*
 * sadicaManager.c
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#include "sadicaManager.h"

bool basicExistFile(fileSystem_struct* fsStruct, char* path) {
	char* fullPath = getFullFilePath(fsStruct, path);
	return getSadicaFile(fsStruct, fullPath) != NULL;
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
			readDataFromFS(fsStruct, file, offset, size, buffer);
		}
	} else {
		*status = EXC_ERROR_FILE_NOT_FOUND;
	}
	free(fullPath);
}

void saveDataOnFS(fileSystem_struct* fsStruct, sadica_file* file, int offset,
		int length, char* buffer) {
	int bufferOffset = 0;
	char* tmpBuffer;
	int firstByte, lastByte, blockNumber, tmpBufferSize;
	int blockSize = getSadicaMetadata()->size;
	int firstBlock = offset / blockSize;
	int lastBlock = (offset + length) / blockSize;
	int firstPageOffset = offset % blockSize;
	int lastPageOffset = (offset + length) % blockSize;
	for (blockNumber = firstBlock; blockNumber <= lastBlock; ++blockNumber) {
		firstByte = (blockNumber == firstBlock) ? firstPageOffset : 0;
		lastByte = (blockNumber == lastBlock) ? lastPageOffset : blockSize;
		tmpBufferSize = lastByte - firstByte;
		tmpBuffer = malloc(tmpBufferSize);
		memcpy(tmpBuffer, buffer + bufferOffset, tmpBufferSize);
		saveDataOnBlock(fsStruct, file, blockNumber, firstByte, tmpBufferSize,
				tmpBuffer);
		bufferOffset += tmpBufferSize;
		free(tmpBuffer);
	}
}

void readDataFromFS(fileSystem_struct* fsStruct, sadica_file* file, int offset,
		int length, char* buffer) {
	int bufferOffset = 0;
	char* tmpBuffer;
	int firstByte, lastByte, blockNumber, tmpBufferSize;
	int blockSize = getSadicaMetadata()->size;
	int firstBlock = offset / blockSize;
	int lastPage = (offset + length) / blockSize;
	int firstPageOffset = offset % blockSize;
	int lastPageOffset = (offset + length) % blockSize;
	for (blockNumber = firstBlock; blockNumber <= lastPage; ++blockNumber) {
		firstByte = (blockNumber == firstBlock) ? firstPageOffset : 0;
		lastByte = (blockNumber == lastPage) ? lastPageOffset : blockSize;
		tmpBufferSize = lastByte - firstByte;
		tmpBuffer = malloc(tmpBufferSize);
		readDataFromBlock(fsStruct, file, blockNumber, firstByte, tmpBufferSize,
				tmpBuffer);
		memcpy(buffer + bufferOffset, tmpBuffer, tmpBufferSize);
		bufferOffset += tmpBufferSize;
		free(tmpBuffer);
	}
}

void saveDataOnBlock(fileSystem_struct* fsStruct, sadica_file* file,
		int blockNumber, int firstByte, int bufferSize, char* buffer) {
	char* path = getBlockFilePath(fsStruct, blockNumber);
	sadica_block* block = createSadicaBlockFrom(fsStruct, path);
	memcpy(block->data, buffer + firstByte, bufferSize);
	writeBlockData(fsStruct, block);
	destroySadicaBlock(fsStruct, block);
	free(path);
}

void readDataFromBlock(fileSystem_struct* fsStruct, sadica_file* file,
		int blockNumber, int firstByte, int bufferSize, char* buffer) {
	char* path = getBlockFilePath(fsStruct, blockNumber);
	sadica_block* block = createSadicaBlockFrom(fsStruct, path);
	memcpy(buffer, block->data + firstByte, bufferSize);
	destroySadicaBlock(fsStruct, block);
	free(path);
}

void validateBlocksQuantity(sadica_file* file, int maxSize, int*status) {
	*status = (file->size >= maxSize) ? EXC_OK : EXC_ERROR_BLOCK_FAULT;
}

void resizeBlocksQuantity(fileSystem_struct* fsStruct, sadica_file* file,
		int maxSize, int* status) {
	uint32_t current = getBlocksQuantity(fsStruct, file->size);
	uint32_t new = getBlocksQuantity(fsStruct, maxSize);
	uint32_t missing = new - current;
	file->blocks = realloc(file->blocks, sizeof(uint32_t) * new);
	assignBlocks(fsStruct, missing, file->blocks + current, status);
	if(*status == EXC_OK){
		file->size = maxSize;
		writeMetadataFile(fsStruct,file);
	}
}
