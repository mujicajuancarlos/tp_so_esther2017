/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"



t_fileDescriptor* createNew_t_fileDescriptor(char* path){

	t_fileDescriptor* newFD = malloc(sizeof(t_fileDescriptor));
	newFD->fd = getFD();
	newFD->path = path;
	newFD->open= 0;

	return newFD;
}

void destroy_t_filedescriptor(t_fileDescriptor* fd){
	free(fd->path);
	free(fd);

}

int getFD(){
	int fd=0;
	int aux;
	aux=fd;
	fd++;
	return aux;
}

