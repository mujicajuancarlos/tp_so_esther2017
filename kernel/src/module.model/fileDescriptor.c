/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"

int main(){


t_fileDescriptor* createNew_t_fileDescriptor(path){

	t_fileDescriptor* newFD = malloc(sizeof(t_fileDescriptor));
	newFD->fd = getFD();
	newFD->path = path;
	newFD->open= 0;

	return newFD;
}

void destroy_t_filedescriptor(fd){
	free(fd);
}

int getFD(){
	int fd=0;
	int aux;
	aux=fd;
	fd++;
	return aux;
}

return 0;
}
