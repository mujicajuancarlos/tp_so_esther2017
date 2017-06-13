/*
 * fileDescriptor.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_FILEDESCRIPTOR_H_
#define MODULE_MODEL_FILEDESCRIPTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <dc-commons/logger.h>
#include <dc-commons/serialization.h>
#include <commons/collections/list.h>

#define FIRST_FD 3
#define READ "r"
#define WRITE "w"
#define RW "rw"

typedef struct {
	uint32_t fd; //numero de FileDescriptor
	char* path;  //Path del archivo
	uint32_t open; //cantidad de procesos que lo tienen abierto
}t_fileDescriptor;

typedef struct {
	bool read;
	bool write;
	//bool create;
}flags;

typedef struct {
	flags* flag;
	t_fileDescriptor* fileDescriptor;
	int offset;//Servira para seek

}t_processFileDescriptor;

t_list* fileDescriptorGlobalList;

void initializeFileSystemModule();
void destroyFileSystemModule();

void tablaGlobalFD_mutex_lock();
void tablaGlobalFD_mutex_unlock();

void fileDescriptor_mutex_lock();
void fileDescriptor_mutex_unlock();


void nextFD_mutex_lock();
void nextFD_mutex_unlock();

t_fileDescriptor* createNew_t_fileDescriptor(char* path);
void destroy_t_filedescriptor(t_fileDescriptor* fd);

int getNextFD();

void imprimirListaDeFD(t_list* lista);

void agregarFD_Alista(t_fileDescriptor* fd);
void removerFD_Lista(t_fileDescriptor* fd);

void incrementarOpen(t_fileDescriptor* fd);
void decrementarOpen(t_fileDescriptor* fd);

void imprimirEstructura(t_fileDescriptor* fd);
void imprimirListaDeFD(t_list* lista);

char* serialize_FileDescriptor(t_fileDescriptor* fd);
size_t sizeOf_FileDescriptor(t_fileDescriptor* fd);
t_fileDescriptor* deserialize_FileDescriptor(char* buffer);
char* getPathFromFD(int fileDesc);
/*
t_processFileDescriptor* createNew_t_processFileDescriptor(char* permiso, t_fileDescriptor* fd);



void destroy_t_processFileDescriptor(t_processFileDescriptor* pfd);

void agregarPFD_Alista (t_processFileDescriptor* pfd);
void removerPFD_Lista(t_processFileDescriptor* pfd);

flags habilitarPermisos(char* permiso);
*/
#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
