/*
 * sadicaBlock.c
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#include "sadicaBlock.h"

sadica_block* createSadicaBlock(char* path) {
	sadica_block* block = malloc(sizeof(sadica_block));
	block->path = string_duplicate(path);
	sadica_metadata* metadata = getSadicaMetadata();
	size_t dataSize = sizeof(char) * metadata->size;
	block->data = malloc(dataSize);
	return block;
}

void destroySadicaBlock(fileSystem_struct* fsStruct, sadica_block* block) {
	if (block->path != NULL) {
		free(block->path);
		block->path = NULL;
	}
	if (block->data != NULL) {
		free(block->data);
		block->data = NULL;
	}
	free(block);
}

sadica_block* createSadicaBlockFrom(fileSystem_struct* fsStruct, char* path) {
	sadica_block* block = createSadicaBlock(path);
	readBlockData(fsStruct, block);
	return block;
}

sadica_block* createSadicaBlockTo(fileSystem_struct* fsStruct, char* path) {
	sadica_block* block = createSadicaBlock(path);
	return block;
}

void readBlockData(fileSystem_struct* fsStruct, sadica_block* block) {
	sadica_metadata* metadata = getSadicaMetadata();
	size_t dataSize = sizeof(char) * metadata->size;
	char* buffer = readFile(block->path, 0, dataSize - 1);
	memcpy(block->data,buffer,dataSize);
	free(buffer);
}

void writeBlockData(fileSystem_struct* fsStruct, sadica_block* block) {
	sadica_metadata* metadata = getSadicaMetadata();
	size_t dataSize = sizeof(char) * metadata->size;
	writeFile(block->data,dataSize,block->path,0);
}
