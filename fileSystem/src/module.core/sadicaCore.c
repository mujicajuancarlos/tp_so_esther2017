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
	logInfo("Iniciando la carga del bitmap SADICA");
	size_t size = getBitsCharSize();
	char* bitschar;
	char* path = getBitmapFilePath(fsStruct);
	if (existFile(path, "r")) {
		logTrace("Leyendo bitmap de %s", path);
		bitschar = readFile(path, 0, size - 1);
		bitmap = bitarray_create_with_mode(bitschar, size, LSB_FIRST);
	} else {
		logTrace(
				"No existe el archivo %s que contiene la informacion del bitmap",
				path);
		bitschar = malloc(sizeof(char) * size);
		bitmap = bitarray_create_with_mode(bitschar, size, LSB_FIRST);
		logTrace("Inicializando el bitmap");
		cleanAllBits(fsStruct);
		logTrace("Guardando bitmap en %s", path);
		writeFile(bitmap->bitarray, size, path, 0);
	}
	free(path);
	logInfo("Finalizó la carga del bitmap SADICA");
}

void loadBlocks(fileSystem_struct* fsStruct) {
	logInfo("Verificando los bloques de datos de SADICA");
	int index;
	char* path = getBlocksDirPath(fsStruct);
	struct stat st = { 0 };
	if (stat(path, &st) == -1) {
		logInfo("No existe el directorio %s", path);
		mkdir(path, 0700);
		logInfo("Se creo el directorio %s", path);
	}
	free(path);
	sadica_block* block;
	for (index = 0; index < metadata.quantity; index++) {
		path = getBlockFilePath(fsStruct, index);
		logDebug("Verificando el bloque %d", index);
		if (!existFile(path, "r")) {
			logTrace("El bloque %d no existe, se creará el bloque en %s", index,
					path);
			block = createSadicaBlockTo(fsStruct, path);
			writeBlockData(fsStruct, block);
			destroySadicaBlock(fsStruct, block);
		}
		free(path);
	}
	logInfo("Finalizó la verificacion de los bloques de datos de SADICA");
}

void loadFiles(fileSystem_struct* fsStruct) {
	logInfo("Iniciando la carga de archivos SADICA");
	sadicaFiles = list_create();
	char* path = getFilesDirPath(fsStruct);
	struct stat st = { 0 };
	if (stat(path, &st) == -1) {
		logInfo("No existe el directorio %s", path);
		mkdir(path, 0700);
		logInfo("Se creo el directorio %s", path);
	}
	logInfo("Buscando archivos sadica en el punto de montaje especificado");
	findSadicaFilesOn(fsStruct, path);
	free(path);
	logInfo("Finalizó la carga de archivos SADICA");
}

void loadMetadata(fileSystem_struct* fsStruct) {
	logInfo("Iniciando la carga de la configuracion de SADICA");
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
	logInfo("Finalizó la carga configuracion de SADICA");
}

/**
 * usado cuando ya existe el archivo
 */
void loadSadicaFile(fileSystem_struct* fsStruct, char* pathFile) {
	logInfo("Cargando el archivo desde %s", pathFile);
	sadica_file* sadicaFile = createSadicaFileFrom(fsStruct, pathFile);
	list_add(sadicaFiles, sadicaFile);
}

/**
 * usado cuando el archivo no existe
 */
void addSadicaFile(fileSystem_struct* fsStruct, char* pathFile, int* status) {
	uint32_t block;
	assignBlocks(fsStruct, 1, &block, status); //asigno por defecto un bloque
	logInfo("Agrego el archivo %s en el bloque #%d", pathFile, block);
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
	logInfo("Removiendo el archivo %s de tamaño %d con %d bloques", file->path,
			file->size, quantity);
	for (blockIndex = 0; blockIndex < quantity; ++blockIndex) {
		blockNumber = file->blocks[blockIndex];
		logTrace("Libero el bloque #%d", blockNumber);
		bitarray_clean_bit(bitmap, blockNumber);
	}
	destroyAndRemoveSadicaFile(fsStruct, file);
	persistBitMap(fsStruct);
}

void findSadicaFilesOn(fileSystem_struct* fsStruct, char* relativePath) {
	DIR *directory;
	struct dirent *dir;
	directory = opendir(relativePath);
	if (directory == NULL) {
		logError("No se pudo abrir %s ", relativePath);
		exit(EXIT_FAILURE);
	}
	while ((dir = readdir(directory))) {
		logInfo("Buscando archivos en el directorio %s", relativePath);
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
			continue;
		}
		char* dirOrFile = string_new();
		string_append(&dirOrFile, relativePath);
		string_append(&dirOrFile, "/");
		string_append(&dirOrFile, dir->d_name);
		if (dir->d_type == DT_DIR) {
			findSadicaFilesOn(fsStruct, dirOrFile);
		} else {
			loadSadicaFile(fsStruct, dirOrFile);
		}
		free(dirOrFile);
	}
	closedir(directory);
}

void assignBlocks(fileSystem_struct* fsStruct, int size, uint32_t* blocks,
		int* status) {
	int index;
	int auxSize = 0;
	if (size > 0) {
		logInfo(
				"Se redimencionara la cantidad de bloques asignados al archivo, se agregaran %d bloques",
				size);
		for (index = 0; (auxSize < size) && (index < metadata.quantity);
				index++) {
			if (bitarray_test_bit(bitmap, index) == 0) {
				logTrace(
						"Se marco el bloque %d como candidato para la solicitud de reserva",
						index);
				memcpy(&(blocks[auxSize]), &index, sizeof(uint32_t));
				auxSize++;
			}
		}
		if (auxSize == size) {
			*status = EXC_OK;
			logTrace(
					"Se encontro la cantidad de bloques solicitados, se persistira la reserva");
			for (index = 0; index < auxSize; index++) {
				if (bitarray_test_bit(bitmap, index) == 0) {
					bitarray_set_bit(bitmap, blocks[index]);
				}
			}
			persistBitMap(fsStruct);
		} else {
			logTrace(
					"NO se encontro la cantidad de bloques solicitados, FILE SYSTEM FULL");
			*status = EXC_ERROR_FILE_SYSTEM_FULL;
		}
	}
}

void unassignBlocks(fileSystem_struct* fsStruct, int size, uint32_t* blocks,
		int* status) {
	int index;
	if (size > 0) {
		logInfo(
				"Se redimencionara la cantidad de bloques asignados al archivo, se reduciran %d bloques",
				size);
		for (index = 0; index < size; index++) {
			if (bitarray_test_bit(bitmap, index) == 0) {
				bitarray_clean_bit(bitmap, blocks[index]);
			}
		}
		persistBitMap(fsStruct);
	}
	*status = EXC_OK;
}

void persistBitMap(fileSystem_struct* fsStruct) {
	size_t size = getBitsCharSize();
	char* path = getBitmapFilePath(fsStruct);
	logTrace("Persistiendo el bitmap en %s", path);
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
	logTrace("Limpiando el bitmap");
	for (index = 0; index < metadata.quantity; index++) {
		bitarray_clean_bit(bitmap, index);
	}
}

void setAllBits(fileSystem_struct* fsStruct) {
	int index;
	for (index = 0; index < metadata.quantity; index++) {
		bitarray_set_bit(bitmap, index);
	}
}

sadica_file* getSadicaFile(fileSystem_struct* fsStruct, char* fullPath) {
	logTrace("Buscando el archivo %s", fullPath);
	bool condition(void* element) {
		sadica_file* file = element;
		return string_equals_ignore_case(file->path, fullPath);
	}
	return list_find(sadicaFiles, condition);
}

sadica_metadata* getSadicaMetadata() {
	return &metadata;
}

char* getSadicaRootPath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, fsStruct->config->punto_montaje);
	if (!string_ends_with(path, PATH_DIV)) {
		string_append(&path, PATH_DIV);
	}
	return path;
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

char* getBlocksDirPath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaRootPath(fsStruct));
	string_append(&path, BLOCK_FILE_PATH);
	return path;
}

char* getBlockFilePath(fileSystem_struct* fsStruct, int index) {
	char *path = getBlocksDirPath(fsStruct);
	string_append(&path, PATH_DIV);
	string_append(&path, string_itoa(index));
	string_append(&path, BLOCK_EXT);
	return path;
}

char* getFilesDirPath(fileSystem_struct* fsStruct) {
	char *path = string_new();
	string_append(&path, getSadicaRootPath(fsStruct));
	string_append(&path, FILES_PATH);
	return path;
}

char* getFullFilePath(fileSystem_struct* fsStruct, char* path) {
	char* fullPath = getFilesDirPath(fsStruct);
	if (!string_starts_with(path, PATH_DIV)) {
		string_append(&path, PATH_DIV);
	}
	string_append(&fullPath, path);
	return fullPath;
}
