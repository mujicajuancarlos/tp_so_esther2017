/*
 * kernel.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

//standar
#include <stdio.h>
#include <stdlib.h>

#include <commons/config.h>
#include <dc-commons/logger.h>
#include "configuration.h"

typedef struct fileSystem_struct{
	int socketServer;
	int fd_kernel;
	Configuration* config;
} fileSystem_struct;

#endif /* FILESYSTEM_H_ */
