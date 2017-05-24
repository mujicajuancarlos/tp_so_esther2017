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

typedef struct {
	uint32_t fd; //numero de FileDescriptor
	char* path;  //Path del archivo
	uint32_t open; //cantidad de procesos que lo tienen abierto
}t_fileDescriptor;

#endif /* MODULE_MODEL_FILEDESCRIPTOR_H_ */
