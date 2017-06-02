/*
 * fileDescriptor.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "fileDescriptor.h"


//t_list* tablaProcesosFD; //TODO: Listo: Agrego tablaProcesosFD a struct process (con nombre fileDescriptorList)
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
/*
t_processFileDescriptor* createNew_t_processFileDescriptor(char* permiso,
		t_fileDescriptor* fd) {

	t_processFileDescriptor* newPFD = malloc(sizeof(t_processFileDescriptor));

	newPFD->fileDescriptor = fd;
	newPFD->flag = habilitarPermisos(permiso);

	return newPFD;

}

void destroy_t_processFileDescriptor(t_processFileDescriptor* pfd) {
	destroy_t_filedescriptor(pfd->fileDescriptor);
	free(pfd);
}


flags habilitarPermisos(char* permiso) {
	flags flag;
	int retorno=-1;
	retorno = strcmp(permiso, READ);
	if (retorno == 0) {
		flag.read = true;
		flag.write = false;
		//logInfo("Los permisos para el FileDescriptor han sido seteados a Read Only");
	} else {
		retorno = strcmp(permiso, WRITE);
		if (retorno == 0) {
			flag.read = false;
			flag.write = true;
			//logInfo("Los permisos para el FileDescriptor %s han sido seteados a Write Only", auxPFD->fileDescriptor->fd);
		} else {
			retorno = strcmp(permiso, RW);
			if (retorno == 0) {
				flag.read = true;
				flag.write = true;
				//logInfo("Los permisos para el FileDescriptor %s han sido seteados a Read & Write", auxPFD->fileDescriptor->fd);
			} else {
				flag.read = false;
				flag.write = false;
				//logInfo("No se han podido setear los permisos correspondientes para el FileDescriptor %s", auxPFD->fileDescriptor->fd);
			}
		}
	}
	return flag;
}

*/
