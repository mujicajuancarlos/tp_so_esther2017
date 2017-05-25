/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"

t_list* tablaGlobalFD;
int nextFD = FIRST_FD;

pthread_mutex_t fileDescriptor_mutex;
pthread_mutex_t tablaGlobalFD_mutex;
pthread_mutex_t nextFD_mutex;

void initializeFileSystemModule() {
	logInfo("Inicializando el modulo FS");
	pthread_mutex_init(&tablaGlobalFD_mutex, NULL);
	pthread_mutex_init(&fileDescriptor_mutex, NULL);
	pthread_mutex_init(&nextFD_mutex, NULL);
	tablaGlobalFD = list_create();
}

void destroyFileSystemModule() {
	logInfo("Destruyendo el modulo FS");
	pthread_mutex_destroy(&tablaGlobalFD_mutex);
	pthread_mutex_destroy(&fileDescriptor_mutex);
	pthread_mutex_destroy(&nextFD_mutex);
	list_destroy_and_destroy_elements(tablaGlobalFD,
			(void*) destroy_t_filedescriptor);
}

void tablaGlobalFD_mutex_lock() {
	pthread_mutex_lock(&tablaGlobalFD_mutex);
}

void tablaGlobalFD_mutex_unlock() {
	pthread_mutex_unlock(&tablaGlobalFD_mutex);
}

void fileDescriptor_mutex_lock() {
	pthread_mutex_lock(&fileDescriptor_mutex);
}

void fileDescriptor_mutex_unlock() {
	pthread_mutex_unlock(&fileDescriptor_mutex);
}

void nextFD_mutex_lock() {
	pthread_mutex_lock(&nextFD_mutex);
}

void nextFD_mutex_unlock() {
	pthread_mutex_unlock(&nextFD_mutex);
}

t_fileDescriptor* createNew_t_fileDescriptor(char* path) {

	t_fileDescriptor* newFD = malloc(sizeof(t_fileDescriptor));
	newFD->fd = getNextFD();
	newFD->open = 0;
	size_t sizeBuffer = strlen(path); //asegurarse que el que lo llame pase un string y no un stream
	newFD->path = calloc(1, sizeBuffer);
	memcpy(newFD->path, path, sizeBuffer);

	return newFD;
}

void destroy_t_filedescriptor(t_fileDescriptor* fd) {
	if (fd->path != NULL) {
		free(fd->path);
	}
	free(fd);
}

int getNextFD() {
	int aux;
	nextFD_mutex_lock();
	aux = nextFD;
	nextFD++;
	nextFD_mutex_unlock();
	return aux;
}

void agregarFD_Alista(t_fileDescriptor* fd) {
	tablaGlobalFD_mutex_lock();
	list_add(tablaGlobalFD, fd);
	tablaGlobalFD_mutex_unlock();
}

void removerFD_Lista(t_fileDescriptor* fd) {
	bool condicion(void* element) {
		t_fileDescriptor* unFD = element;
		return unFD == fd;
	}
	tablaGlobalFD_mutex_lock();
	list_remove_and_destroy_by_condition(tablaGlobalFD, condicion,
			(void*) destroy_t_filedescriptor);
	tablaGlobalFD_mutex_unlock();
}

void incrementarOpen(t_fileDescriptor* fd) {
	fileDescriptor_mutex_lock();
	fd->open = fd->open + 1;
	fileDescriptor_mutex_unlock();
}

void decrementarOpen(t_fileDescriptor* fd) {
	fileDescriptor_mutex_lock();
	fd->open--;
	if (fd->open == 0) {
		removerFD_Lista(fd);
	}
	fileDescriptor_mutex_unlock();
}

