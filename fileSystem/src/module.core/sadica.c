/*
 * sadica.c
 *
 *  Created on: 20/6/2017
 *      Author: utnso
 */

#include "sadica.h"

sadica_metadata metadata;

void initializeSadicaFileSystem(fileSystem_struct* fsStruct) {
	char* path = getMetadataFilePath(fsStruct);
	t_config* config = config_create(path);
	if (config == NULL) {
		logError("No se pudo cargar los metadatos de sadica desde el path %s", path);
		exit(1);
	}
	config_set_int_valid_value(&metadata.size, config, TAMANIO_BLOQUES);
	config_set_int_valid_value(&metadata.quantity, config, CANTIDAD_BLOQUES);
	config_set_string_valid_value(&metadata.magicNumber, config, MAGIC_NUMBER);
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
