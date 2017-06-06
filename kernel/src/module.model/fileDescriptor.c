/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"


int nextFD = FIRST_FD;

pthread_mutex_t fileDescriptor_mutex;
pthread_mutex_t tablaGlobalFD_mutex;
pthread_mutex_t nextFD_mutex;


void initializeFileSystemModule() {
	logInfo("Inicializando el modulo FS");
	pthread_mutex_init(&tablaGlobalFD_mutex, NULL);
	pthread_mutex_init(&fileDescriptor_mutex, NULL);
	pthread_mutex_init(&nextFD_mutex, NULL);

	fileDescriptorGlobalList = list_create();

}

void destroyFileSystemModule() {
	logInfo("Destruyendo el modulo FS");
	pthread_mutex_destroy(&tablaGlobalFD_mutex);
	pthread_mutex_destroy(&fileDescriptor_mutex);
	pthread_mutex_destroy(&nextFD_mutex);
	list_destroy_and_destroy_elements(fileDescriptorGlobalList,
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
	size_t sizeBuffer = strlen(path); //TODO: asegurarse que el que lo llame pase un string y no un stream
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
	list_add(fileDescriptorGlobalList, fd);
	tablaGlobalFD_mutex_unlock();
}

void removerFD_Lista(t_fileDescriptor* fd) {
	bool condicion(void* element) {
		t_fileDescriptor* unFD = element;
		return unFD == fd;
	}
	tablaGlobalFD_mutex_lock();
	list_remove_and_destroy_by_condition(fileDescriptorGlobalList, condicion,
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

void imprimirEstructura(t_fileDescriptor* fd) {
	printf("Estructura FD:\n File Descriptor: %d\n,Archivo: %s\n,Open: %d\n",
			fd->fd, fd->path, fd->open);

}

void imprimirListaDeFD(t_list* lista) {
	fileDescriptor_mutex_lock();
	t_link_element* aux = lista->head;
	while (aux != NULL) {
		imprimirEstructura(aux->data);
		aux = aux->next;
	}
	fileDescriptor_mutex_unlock();
}

size_t sizeOf_FileDescriptor(t_fileDescriptor* fd) {
	size_t size = 0;
	size += sizeof(uint32_t) * 2;
	size += sizeof(char) * fd->fd;
	return size;
}
char* serialize_FileDescriptor(t_fileDescriptor* fd) {
	char* buffer = malloc(sizeOf_FileDescriptor(fd));

	uint32_t offset = 0;

	serialize_and_copy_value(buffer, &(fd->fd), sizeof(uint32_t),
			&offset);

	serialize_and_copy_value(buffer, fd->path,
			sizeof(char) * fd->fd, &offset);

	serialize_and_copy_value(buffer, &(fd->open), sizeof(uint32_t),
			&offset);

	return buffer;
}
t_fileDescriptor* deserialize_FileDescriptor(char* buffer) {

	t_fileDescriptor* fd = malloc(sizeof(t_fileDescriptor));

	uint32_t offset = 0;

	deserialize_and_copy_value(&(fd->fd), buffer, sizeof(uint32_t),
			&offset);

	fd->path = malloc(sizeof(char) * fd->fd);

	deserialize_and_copy_value(fd->path, buffer,
			sizeof(char) * fd->fd, &offset);

	deserialize_and_copy_value(&(fd->open), buffer, sizeof(uint32_t),
			&offset);
	return fd;
}












