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
#include <commons/collections/list.h>

#define FIRST_FD 3

typedef struct {
	uint32_t fd; //numero de FileDescriptor
	char* path;  //Path del archivo
	uint32_t open; //cantidad de procesos que lo tienen abierto
}t_fileDescriptor;

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

void agregarFD_Alista(t_fileDescriptor* fd);
void removerFD_Lista(t_fileDescriptor* fd);

void incrementarOpen(t_fileDescriptor* fd);
void decrementarOpen(t_fileDescriptor* fd);

#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
