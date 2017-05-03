/*
 * fsStruct.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef FILESYSTEMSTRUCT_H_
#define FILESYSTEMSTRUCT_H_

#include "configuration.h"

typedef struct fileSystem_struct{
	int socketServer;
	int fd_kernel;
	Configuration* config;
} fileSystem_struct;

#endif /* FILESYSTEMSTRUCT_H_ */
