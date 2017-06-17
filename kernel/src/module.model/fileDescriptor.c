/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"

int currentFD = FIRST_PROCESS_FD;

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
	size_t sizePath = sizeof(char)*strlen(path);
	t_globalFile* globalFile = malloc(sizeof(t_globalFile));
	globalFile->open = 0;
	globalFile->path = malloc(sizePath);
	memcpy(globalFile->path, path, sizePath);
	return globalFile;
}
void destroy_t_globalFile(t_globalFile* globalFile) {
	if (globalFile->path != NULL) {
		free(globalFile->path);
		globalFile->path = NULL;
	}
	free(globalFile);
}

t_processFile* create_t_processFile(uint32_t fd_number, t_globalFile* globalFile, flags flags){
	t_processFile* processFile = malloc(sizeof(t_processFile));
	processFile->fd = fd_number;
	processFile->flag = flags;
	processFile->globalFile = globalFile;
	globalFile->open = globalFile->open + 1;
	processFile->seekValue = 0;
	return processFile;
}
void destroy_t_processFile(t_processFile* processFile){
	processFile->globalFile->open = processFile->globalFile->open - 1;
	processFile->globalFile = NULL;
	free(processFile);
}

void addGlobalFile(t_globalFile* globalFile) {
	globalFilesMutex_lock();
	list_add(globalFiles, globalFile);
	globalFilesMutex_unlock();
}

void removeClosedGlobalFiles() {
	bool condicion(void* element) {
		t_globalFile* globalFile = element;
		return globalFile->open == 0;
	}
	globalFilesMutex_lock();
	list_remove_and_destroy_by_condition(globalFiles, condicion,
			(void*) destroy_t_globalFile);
	globalFilesMutex_unlock();
}
