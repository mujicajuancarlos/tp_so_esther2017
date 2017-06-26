/*
 * sadicaManager.h
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_SADICAMANAGER_H_
#define MODULE_CORE_SADICAMANAGER_H_

#include <stdbool.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include "fsStatusCodes.h"
#include "sadicaCore.h"

bool basicExistFile(fileSystem_struct* fsStruct, char* path);
void basicDeleteFile(fileSystem_struct* fsStruct, char* path, int* status);
void basicCreateFile(fileSystem_struct* fsStruct, char* path, int*status);
void basicWriteFile(fileSystem_struct* fsStruct, char* path, int offset,
		int size, char* buffer, int*status);
void basicReadFile(fileSystem_struct* fsStruct, char* path, int offset,
		int size, char* buffer, int*status);

void saveDataOnFS(fileSystem_struct* fsStruct,sadica_file* file,int offset,
		int size, char* buffer);
void readDataOnFS(fileSystem_struct* fsStruct,sadica_file* file,int offset,
		int size, char* buffer);

void validateBlocksQuantity(sadica_file* file, int maxSize, int*status);
void resizeBlocksQuantity(fileSystem_struct* fsStruct, sadica_file* file,
		int maxSize, int* status);

#endif /* MODULE_CORE_SADICAMANAGER_H_ */
