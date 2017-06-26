/*
 * sadica.h
 *
 *  Created on: 20/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_SADICACORE_H_
#define MODULE_CORE_SADICACORE_H_

#include <dirent.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include <dc-commons/config.h>
#include <dc-commons/utils.h>
#include "../fileSystemStruct.h"
#include "fsStatusCodes.h"
#include "sadicaFile.h"
#include "sadicaBlock.h"

#define METADATA_FILE_PATH "metadata/metadata.bin"
#define BITMAP_FILE_PATH "metadata/bitmap.bin"
#define BLOCK_FILE_PATH "blocks/"
#define FILES_PATH "files/"
#define BLOCK_EXT ".bin"

#define MAGIC_NUMBER_VALUE "SADICA"
#define TAMANIO_BLOQUES "TAMANIO_BLOQUES"
#define CANTIDAD_BLOQUES "CANTIDAD_BLOQUES"
#define MAGIC_NUMBER "MAGIC_NUMBER"

//file metadata
#define TAMANIO "TAMANIO"
#define BLOQUES "BLOQUES"

typedef struct {
	int size;
	int quantity;
	char* magicNumber;
} sadica_metadata;


void initializeSadicaFileSystem(fileSystem_struct* fsStruct);

void loadMetadata(fileSystem_struct* fsStruct);
void loadBitmap(fileSystem_struct* fsStruct);
void loadBlocks(fileSystem_struct* fsStruct);
void loadFiles(fileSystem_struct* fsStruct);

void loadSadicaFile(fileSystem_struct* fsStruct, char* pathFile);
void addSadicaFile(fileSystem_struct* fsStruct, char* pathFile, int* status);
void removeSadicaFile(fileSystem_struct* fsStruct, sadica_file* file);

void findSadicaFilesOn(fileSystem_struct* fsStruct, char* relativePath);

void assignBlocks(fileSystem_struct* fsStruct, int size, int* blocks,int* status);
void persistBitMap(fileSystem_struct* fsStruct);
size_t getBitsCharSize();

void cleanAllBits(fileSystem_struct* fsStruct);
void setAllBits(fileSystem_struct* fsStruct);

sadica_file* getSadicaFile(fileSystem_struct* fsStruct, char* path);
sadica_metadata* getSadicaMetadata();

char* getSadicaRootPath(fileSystem_struct* fsStruct);
char* getMetadataFilePath(fileSystem_struct* fsStruct);
char* getBitmapFilePath(fileSystem_struct* fsStruct);
char* getBlockFilePath(fileSystem_struct* fsStruct, int index);
char* getFilesPath(fileSystem_struct* fsStruct);
char* getFullFilePath(fileSystem_struct* fsStruct, char* path);

#endif /* MODULE_CORE_SADICACORE_H_ */
