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
#include <dc-commons/socket-server.h>
#include "handler-kernel.h"
#include "fileSystemStruct.h"

void createKernelServerSocket(fileSystem_struct* fsStruct);

void initializeStruct(fileSystem_struct* fsStruct, Configuration* config);

#endif /* FILESYSTEM_H_ */
