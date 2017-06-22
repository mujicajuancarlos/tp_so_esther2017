/*
 * sadica.h
 *
 *  Created on: 20/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_SADICA_H_
#define MODULE_CORE_SADICA_H_

#include <commons/string.h>
#include <dc-commons/logger.h>
#include <dc-commons/config.h>
#include "../fileSystemStruct.h"

#define METADATA_FILE_PATH "metadata/metadata.bin"
#define BITMAP_FILE_PATH "metadata/bitmap.bin"

#define TAMANIO_BLOQUES "TAMANIO_BLOQUES"
#define CANTIDAD_BLOQUES "CANTIDAD_BLOQUES"
#define MAGIC_NUMBER "MAGIC_NUMBER"

//file metadata
#define TAMANIO "TAMANIO"
#define BLOQUES "BLOQUES"

typedef struct {
	int size;
	int* blocks;
} file_metadata;

typedef struct {
	int size;
	int quantity;
	char* magicNumber;
} sadica_metadata;


void initializeSadicaFileSystem(fileSystem_struct* fsStruct);

void loadMetadata(fileSystem_struct* fsStruct);

char* getSadicaPath(fileSystem_struct* fsStruct);
char* getMetadataFilePath(fileSystem_struct* fsStruct);
char* getBitmapFilePath(fileSystem_struct* fsStruct);

#endif /* MODULE_CORE_SADICA_H_ */
