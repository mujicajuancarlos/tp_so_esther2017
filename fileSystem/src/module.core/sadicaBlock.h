/*
 * sadicaBlock.h
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_SADICABLOCK_H_
#define MODULE_CORE_SADICABLOCK_H_

#include <string.h>
#include <stdlib.h>
#include <dc-commons/utils.h>
#include <dc-commons/serialization.h>
#include "../fileSystemStruct.h"
#include "sadicaCore.h"

typedef struct {
	char* path;
	char* data;
} sadica_block;

sadica_block* createSadicaBlock(char* path);
void destroySadicaBlock(fileSystem_struct* fsStruct, sadica_block* block);

sadica_block* createSadicaBlockFrom(fileSystem_struct* fsStruct, char* path);
sadica_block* createSadicaBlockTo(fileSystem_struct* fsStruct, char* path);

void readBlockData(fileSystem_struct* fsStruct, sadica_block* block);
void writeBlockData(fileSystem_struct* fsStruct, sadica_block* block);

#endif /* MODULE_CORE_SADICABLOCK_H_ */
