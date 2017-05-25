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
#include <commons/collections/list.h>

typedef struct {
	uint32_t fd; //numero de FileDescriptor
	char* path;  //Path del archivo
	uint32_t open; //cantidad de procesos que lo tienen abierto
}t_fileDescriptor;


void initializefileDescriptors();

void destroyfileDescriptors();

void fileDescriptors_lock();

void fileDescriptors_unlock();

t_fileDescriptor* createNew_t_fileDescriptor(char*);
void destroy_t_filedescriptor(t_fileDescriptor*);
int getFD();
#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
