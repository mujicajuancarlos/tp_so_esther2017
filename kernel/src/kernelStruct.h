/*
 * kernelStruct.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef KERNELSTRUCT_H_
#define KERNELSTRUCT_H_

#include <pthread.h>
#include <commons/collections/list.h>
#include "configuration.h"

//estructura princial del kernel
typedef struct kernel_struct {
	int socketServerCPU;
	int socketServerConsola;
	int socketClientMemoria;
	int socketClientFileSystem;
	int pageSize;
	pthread_mutex_t stdoutMutex;
	Configuration* config;
} kernel_struct;

#endif /* KERNELSTRUCT_H_ */
