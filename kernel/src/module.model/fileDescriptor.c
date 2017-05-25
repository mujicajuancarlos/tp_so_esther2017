/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"

t_list* tablaGlobalFD;
static int fd=3;
static int aux;

pthread_mutex_t fileDescriptorMutex;
pthread_mutex_t tablaGlobalFDMutex;

void initializetablaGlobalFDMutex() {
	pthread_mutex_init(&tablaGlobalFDMutex, NULL);
}

void destroytablaGlobalFDMutex() {
	pthread_mutex_destroy(&tablaGlobalFDMutex);
}

void tablaGlobalFDMutex_lock() {
	pthread_mutex_lock(&tablaGlobalFDMutex);

}

void tablaGlobalFDMutex_unlock() {
	pthread_mutex_unlock(&tablaGlobalFDMutex);

}

void initializefileDescriptors() {
	pthread_mutex_init(&fileDescriptorMutex, NULL);
}

void destroyfileDescriptors() {
	pthread_mutex_destroy(&fileDescriptorMutex);
}

void fileDescriptors_lock() {
	pthread_mutex_lock(&fileDescriptorMutex);

}

void fileDescriptors_unlock() {
	pthread_mutex_unlock(&fileDescriptorMutex);

}

t_fileDescriptor* createNew_t_fileDescriptor(char* path){

	t_fileDescriptor* newFD = malloc(sizeof(t_fileDescriptor));
	newFD->fd = getFD();
	newFD->path = path;
	newFD->open= 0;

	return newFD;
}

void destroy_t_filedescriptor(t_fileDescriptor* fd){
	if(fd->path != NULL){

		free(fd->path);
	}
	free(fd);

};

int getFD(){
	fileDescriptors_lock();
	aux=fd;
	fd++;
	fileDescriptors_unlock();
	return aux;
}



t_list* inicializarTablaGlobal(){
	tablaGlobalFD = list_create();
	return tablaGlobalFD;
}


void agregarFD_Alista(t_fileDescriptor* fd,t_list* lista){
	tablaGlobalFDMutex_lock();
	list_add(lista, fd);
	tablaGlobalFDMutex_unlock();
}


void removerFD_Lista(t_fileDescriptor* fd, t_list* lista){
	bool condicion(void* element){
		t_fileDescriptor* unFD = element;
		return unFD->path == fd->path;
	}
	tablaGlobalFDMutex_lock();
	list_remove_and_destroy_by_condition(lista,condicion,destroy_t_filedescriptor);
	tablaGlobalFDMutex_unlock();
}

void incrementarOpen(t_fileDescriptor* fd){
	tablaGlobalFDMutex_lock();
	fd->open = fd->open +1;
	tablaGlobalFDMutex_unlock();
}

void decrementarOpen(t_fileDescriptor* fd){
	tablaGlobalFDMutex_lock();
	if(fd->open <=1){
		removerFD_Lista(fd,tablaGlobalFD);
	}
	fd->open = fd->open -1;
	tablaGlobalFDMutex_unlock();
}

