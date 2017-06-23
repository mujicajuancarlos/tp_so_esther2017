/*
 * sadica.h
 *
 *  Created on: 20/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_SADICA_H_
#define MODULE_CORE_SADICA_H_

#include <dirent.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include <dc-commons/config.h>
#include <dc-commons/utils.h>
#include "../fileSystemStruct.h"
#include "sadicaFile.h"


#define METADATA_FILE_PATH "metadata/metadata.bin"
#define BITMAP_FILE_PATH "metadata/bitmap.bin"
#define BLOCK_FILE_PATH "blocks/"
#define FILES_PATH "files/"
#define BLOCK_EXT ".bin"

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

void findSadicaFilesOn(fileSystem_struct* fsStruct, char* relativePath);

size_t getBitsCharSize();

void cleanAllBits(fileSystem_struct* fsStruct);
void setAllBits(fileSystem_struct* fsStruct);

sadica_metadata* getSadicaMetadata();

char* getSadicaPath(fileSystem_struct* fsStruct);
char* getMetadataFilePath(fileSystem_struct* fsStruct);
char* getBitmapFilePath(fileSystem_struct* fsStruct);
char* getBlockFilePath(fileSystem_struct* fsStruct, int index);
char* getFilesPath(fileSystem_struct* fsStruct);

#endif /* MODULE_CORE_SADICA_H_ */
