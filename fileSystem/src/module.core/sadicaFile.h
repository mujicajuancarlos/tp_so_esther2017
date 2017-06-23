/*
 * sadicaFile.h
 *
 *  Created on: 22/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_SADICAFILE_H_
#define MODULE_CORE_SADICAFILE_H_

#include <string.h>
#include <stdlib.h>
#include <dc-commons/utils.h>
#include <dc-commons/serialization.h>
#include "../fileSystemStruct.h"
#include "sadica.h"

typedef struct {
	char* path;
	uint32_t size;
	uint32_t* blocks;
} sadica_file;

sadica_file* createSadicaFile(char* path);

sadica_file* createSadicaFileFrom(fileSystem_struct* fsStruct, char* path);
sadica_file* createSadicaFileTo(fileSystem_struct* fsStruct, char* path,
		uint32_t firstBlock);

void readMetadataFile(fileSystem_struct* fsStruct, sadica_file* file);
void writeMetadataFile(fileSystem_struct* fsStruct, sadica_file* file);

uint32_t getBlocksQuantity(fileSystem_struct* fsStruct, uint32_t size);

#endif /* MODULE_CORE_SADICAFILE_H_ */
