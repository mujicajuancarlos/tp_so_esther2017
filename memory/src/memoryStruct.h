/*
 * memoryStruct.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MEMORYSTRUCT_H_
#define MEMORYSTRUCT_H_

#include <commons/collections/list.h>
#include "configuration.h"

typedef struct memory_struct {
	int socketServer;
	int socketClientKernel;
	pthread_mutex_t socketClientKernelMutex;
	t_list* listaCPUs;
	pthread_mutex_t listaCPUsMutex;
	Configuration* config;
} memory_struct;

#endif /* MEMORYSTRUCT_H_ */
