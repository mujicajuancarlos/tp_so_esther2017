/*
 * sadicaFile.c
 *
 *  Created on: 22/6/2017
 *      Author: utnso
 */

#include "sadicaFile.h"

sadica_file* createSadicaFile(char* path) {
	sadica_file* file = malloc(sizeof(sadica_file));
	size_t size = sizeof(char) * strlen(path);
	file->path = malloc(size);
	memcpy(file->path, path, size);
	file->size = 0;
	file->blocks = NULL;
	return file;
}

sadica_file* createSadicaFileFrom(fileSystem_struct* fsStruct, char* path) {
	sadica_file* file = createSadicaFile(path);
	readMetadataFile(fsStruct, file);
	return file;
}

sadica_file* createSadicaFileTo(fileSystem_struct* fsStruct, char* path,
		uint32_t firstBlock) {
	sadica_file* file = createSadicaFile(path);
	memcpy(file->blocks, &firstBlock, sizeof(uint32_t));
	writeMetadataFile(fsStruct, file);
	return file;
}

void readMetadataFile(fileSystem_struct* fsStruct, sadica_file* file) {
	long lastByte = fileSize(file->path);
	char* buffer = readFile(file->path, 0, lastByte);
	uint32_t offset = 0;
	deserialize_and_copy_value(&file->size, buffer, sizeof(uint32_t), &offset);
	uint32_t quantity = getBlocksQuantity(fsStruct, file->size);
	uint32_t size = sizeof(uint32_t) * quantity;
	file->blocks = malloc(size);
	deserialize_and_copy_value(file->blocks, buffer, size, &offset);
	free(buffer);
}

void writeMetadataFile(fileSystem_struct* fsStruct, sadica_file* file) {
	uint32_t quantity = getBlocksQuantity(fsStruct, file->size);
	uint32_t size = sizeof(uint32_t) * quantity;
	uint32_t sizeBuffer = size + sizeof(uint32_t);
	char* buffer = malloc(sizeBuffer);
	uint32_t offset = 0;
	serialize_and_copy_value(buffer, &file->size, sizeof(uint32_t), &offset);
	serialize_and_copy_value(buffer, file->blocks, size, &offset);
	writeFile(buffer, sizeBuffer, file->path, 0);
	free(buffer);
}

uint32_t getBlocksQuantity(fileSystem_struct* fsStruct, uint32_t size) {
	if (size == 0)
		return 1;
	sadica_metadata* metadata = getSadicaMetadata();
	uint32_t quantity = (size % metadata->size > 0) ? 1 : 0;
	quantity += size / metadata->size;
	return quantity;
}
