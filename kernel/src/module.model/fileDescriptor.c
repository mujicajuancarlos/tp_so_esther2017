/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"

t_list* globalFiles;
pthread_mutex_t globalFilesMutex;

void initializeFileSystemModule() {
	logInfo("Inicializando el modulo FS");
	pthread_mutex_init(&globalFilesMutex, NULL);
	globalFiles = list_create();
}

void destroyFileSystemModule() {
	logInfo("Destruyendo el modulo FS");
	pthread_mutex_destroy(&globalFilesMutex);
	list_destroy_and_destroy_elements(globalFiles,
			(void*) destroy_t_globalFile);
}

void globalFilesMutex_lock() {
	pthread_mutex_lock(&globalFilesMutex);
}
void globalFilesMutex_unlock() {
	pthread_mutex_unlock(&globalFilesMutex);
}

t_globalFile* create_t_globalFile(char* path) {
	t_globalFile* globalFile = malloc(sizeof(t_globalFile));
	globalFile->open = 0;
	globalFile->path = string_duplicate(path);
	return globalFile;
}
void destroy_t_globalFile(t_globalFile* globalFile) {
	if (globalFile->path != NULL) {
		free(globalFile->path);
		globalFile->path = NULL;
	}
	free(globalFile);
}

t_processFile* create_t_processFile(uint32_t fd_number,
		t_globalFile* globalFile, t_flags flags) {
	t_processFile* processFile = malloc(sizeof(t_processFile));
	processFile->fd = fd_number;
	processFile->flag.read = flags.read;
	processFile->flag.write = flags.write;
	processFile->globalFile = globalFile;
	globalFile->open = globalFile->open + 1;
	processFile->seekValue = 0;
	return processFile;
}
void destroy_t_processFile(t_processFile* processFile) {
	processFile->globalFile->open = processFile->globalFile->open - 1;
	processFile->globalFile = NULL;
	free(processFile);
}

void addGlobalFile(t_globalFile* globalFile) {
	list_add(globalFiles, globalFile);
}

void removeClosedGlobalFiles() {
	bool condition(void* element) {
		t_globalFile* globalFile = element;
		return globalFile->open == 0;
	}
	list_remove_and_destroy_by_condition(globalFiles, condition,
			(void*) destroy_t_globalFile);
}

t_globalFile* getGlobalFileFor(char* path) {
	bool condition(void* element) {
		t_globalFile* globalFile = element;
		return string_equals_ignore_case(path,globalFile->path);
	}
	return list_find(globalFiles, condition);
}
