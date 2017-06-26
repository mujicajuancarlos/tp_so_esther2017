/*
 * sadica.c
 *
 *  Created on: 20/6/2017
 *      Author: utnso
 */

#include "sadicaCore.h"

sadica_metadata metadata;
t_bitarray* bitmap;
t_list* sadicaFiles;

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
	char* path;
	sadica_block* block;
	for (index = 0; index < metadata.quantity; index++) {
		path = getBlockFilePath(fsStruct, index);
		if (!existFile(path, "w")) {
			block = createSadicaBlockTo(fsStruct, path);
			writeBlockData(fsStruct, block);
			destroySadicaBlock(fsStruct, block);
		}
		free(path);
	}
}

void loadFiles(fileSystem_struct* fsStruct) {
	sadicaFiles = list_create();
	char* path = getFilesPath(fsStruct);
	logInfo("Buscando archivos sadica en el punto de montaje especificado");
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
	if (!string_equals_ignore_case(metadata.magicNumber, MAGIC_NUMBER_VALUE)) {
		logError("El magic number es incorrecto");
		exit(EXIT_FAILURE);
	}
}

void loadSadicaFile(fileSystem_struct* fsStruct, char* pathFile) {
	logInfo("Cargando el archivo %s", pathFile);
	sadica_file* sadicaFile = createSadicaFileFrom(fsStruct, pathFile);
	list_add(sadicaFiles, sadicaFile);
}

void addSadicaFile(fileSystem_struct* fsStruct, char* pathFile, int* status) {
	int block;
	assignBlocks(fsStruct, 1, &block, status);//asigno por defecto un bloque
	if (*status == EXC_OK) {
		sadica_file* sadicaFile = createSadicaFileTo(fsStruct, pathFile, block);
		list_add(sadicaFiles, sadicaFile);
	}
}

void removeSadicaFile(fileSystem_struct* fsStruct, sadica_file* file) {
	bool condition(void* element) {
		char* anyPath = element;
		return string_equals_ignore_case(anyPath, file->path);
	}
	list_remove_by_condition(sadicaFiles, condition);
	uint32_t quantity = getBlocksQuantity(fsStruct, file->size);
	uint32_t blockIndex, blockNumber;
	for (blockIndex = 0; blockIndex < quantity; ++blockIndex) {
		blockNumber = file->blocks[blockIndex];
		bitarray_clean_bit(bitmap, blockNumber);
	}
	destroyAndRemoveSadicaFile(fsStruct, file);
	persistBitMap(fsStruct);
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
		logInfo("Buscando archivos el directorio %s", relativePath);
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
			continue;
		}
		char* dirOrFile = string_new();
		string_append(&dirOrFile, relativePath);
		string_append(&dirOrFile, dir->d_name);
		if (dir->d_type == DT_DIR) {
			string_append(&dirOrFile, "/");
			findSadicaFilesOn(fsStruct, dirOrFile);
		} else {
			loadSadicaFile(fsStruct, dirOrFile);
		}
		free(dirOrFile);
	}
	closedir(d);
}

void assignBlocks(fileSystem_struct* fsStruct, int size, int* blocks,
		int* status) {
	int index;
	int auxSize = 0;
	for (index = 0; (auxSize < size) && (index < bitmap->size); index++) {
		if (bitarray_test_bit(bitmap, index) == 0) {
			memcpy(&(blocks[auxSize]), &index, sizeof(uint32_t));
			auxSize++;
		}
	}
	if (auxSize == size) {
		*status = EXC_OK;
		for (index = 0; index < auxSize; index++) {
			if (bitarray_test_bit(bitmap, index) == 0) {
				bitarray_set_bit(bitmap, blocks[index]);
			}
		}
		persistBitMap(fsStruct);
	} else {
		*status = EXC_ERROR_FILE_SYSTEM_FULL;
	}
}

void persistBitMap(fileSystem_struct* fsStruct) {
	size_t size = getBitsCharSize();
	char* path = getBitmapFilePath(fsStruct);
	writeFile(bitmap->bitarray, size, path, 0);
	free(path);
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

sadica_file* getSadicaFile(fileSystem_struct* fsStruct, char* fullPath) {
	logTrace("Buscando el archivo %s", fullPath);
	bool condition(void* element) {
		char* anyPath = element;
		return string_equals_ignore_case(anyPath, fullPath);
	}
	return list_find(sadicaFiles, condition);
}

sadica_metadata* getSadicaMetadata() {
	return &metadata;
}

char* getSadicaRootPath(fileSystem_struct* fsStruct) {
	return fsStruct->config->punto_montaje;
}

char* getMetadataFilePath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaRootPath(fsStruct));
	string_append(&path, METADATA_FILE_PATH);
	return path;
}

char* getBitmapFilePath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaRootPath(fsStruct));
	string_append(&path, BITMAP_FILE_PATH);
	return path;
}

char* getBlockFilePath(fileSystem_struct* fsStruct, int index) {
	char *path = string_new();
	string_append(&path, getSadicaRootPath(fsStruct));
	string_append(&path, BLOCK_FILE_PATH);
	string_append(&path, string_itoa(index));
	string_append(&path, BLOCK_EXT);
	return path;
}

char* getFilesPath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaRootPath(fsStruct));
	string_append(&path, FILES_PATH);
	return path;
}

char* getFullFilePath(fileSystem_struct* fsStruct, char* path) {
	char* fullPath = getFilesPath(fsStruct);
	string_append(&fullPath, path);
	return fullPath;
}
