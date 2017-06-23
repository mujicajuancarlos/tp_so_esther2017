/*
 * sadica.c
 *
 *  Created on: 20/6/2017
 *      Author: utnso
 */

#include "sadica.h"

sadica_metadata metadata;
t_bitarray* bitmap;

void initializeSadicaFileSystem(fileSystem_struct* fsStruct) {
	loadMetadata(fsStruct);
	loadBitmap(fsStruct);
	loadBlocks(fsStruct);
	loadFiles(fsStruct);
}

void loadBitmap(fileSystem_struct* fsStruct) {
	size_t size = getBitsCharSize();
	char* bitschar;
	char* path = getBitmapFilePath(fsStruct);
	if (existFile(path, "r")) {
		bitschar = readFile(path, 0, size - 1);
		bitmap = bitarray_create_with_mode(bitschar, size, LSB_FIRST);
	} else {
		bitschar = malloc(sizeof(char) * size);
		bitmap = bitarray_create_with_mode(bitschar, size, LSB_FIRST);
		cleanAllBits(fsStruct);
		writeFile(bitmap->bitarray, size, path, 0);
	}
	free(path);
}

void loadBlocks(fileSystem_struct* fsStruct) {
	int index;
	char* dataBuffer = calloc(1, metadata.size);
	for (index = 0; index < metadata.quantity; index++) {
		char* path = getBlockFilePath(fsStruct, index);
		if (!existFile(path, "w")) {
			writeFile(dataBuffer, metadata.size, path, 0);
		}
		free(path);
	}
	free(dataBuffer);
}

void loadFiles(fileSystem_struct* fsStruct) {
	char* path = getFilesPath(fsStruct);
	findSadicaFilesOn(fsStruct, path);
	free(path);
}

void loadMetadata(fileSystem_struct* fsStruct) {
	char* path = getMetadataFilePath(fsStruct);
	t_config* config = config_create(path);
	if (config == NULL) {
		logError("No se pudo cargar los metadatos de sadica desde el path %s",
				path);
		exit(EXIT_FAILURE);
	}
	free(path);
	config_set_int_valid_value(&metadata.size, config, TAMANIO_BLOQUES);
	config_set_int_valid_value(&metadata.quantity, config, CANTIDAD_BLOQUES);
	config_set_string_valid_value(&metadata.magicNumber, config, MAGIC_NUMBER);
}

void loadSadicaFile(fileSystem_struct* fsStruct, char* pathFile){
	printf("\nArchivo: %s", pathFile);
}

void findSadicaFilesOn(fileSystem_struct* fsStruct, char* relativePath) {
	DIR *d;
	struct dirent *dir;
	d = opendir(relativePath);
	if (d == NULL) {
		logError("No se pudo abrir %s ", relativePath);
		exit(EXIT_FAILURE);
	}
	while ((dir = readdir(d))) {
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
			continue;
		}
		char* dirOrFile = string_new();
		string_append(&dirOrFile, relativePath);
		string_append(&dirOrFile, dir->d_name);
		if (dir->d_type == DT_DIR) {
			string_append(&dirOrFile,"/");
			findSadicaFilesOn(fsStruct, dirOrFile);
		} else {
			loadSadicaFile(fsStruct, dirOrFile);
		}
		free(dirOrFile);
	}
	closedir(d);
}

size_t getBitsCharSize() {
	size_t size = (metadata.quantity % CHAR_BIT) == 0 ? 0 : 1;
	size += metadata.quantity / CHAR_BIT;
	return size;
}

void cleanAllBits(fileSystem_struct* fsStruct) {
	int index;
	for (index = 0; index < bitmap->size; index++) {
		bitarray_clean_bit(bitmap, index);
	}
}

void setAllBits(fileSystem_struct* fsStruct) {
	int index;
	for (index = 0; index < bitmap->size; index++) {
		bitarray_set_bit(bitmap, index);
	}
}

char* getSadicaPath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, fsStruct->config->relativePath);
	string_append(&path, fsStruct->config->punto_montaje);
	return path;
}

char* getMetadataFilePath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaPath(fsStruct));
	string_append(&path, METADATA_FILE_PATH);
	return path;
}

char* getBitmapFilePath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaPath(fsStruct));
	string_append(&path, BITMAP_FILE_PATH);
	return path;
}

char* getBlockFilePath(fileSystem_struct* fsStruct, int index) {
	char *path = string_new();
	string_append(&path, getSadicaPath(fsStruct));
	string_append(&path, BLOCK_FILE_PATH);
	string_append(&path, string_itoa(index));
	string_append(&path, BLOCK_EXT);
	return path;
}

char* getFilesPath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaPath(fsStruct));
	string_append(&path, FILES_PATH);
	return path;
}
