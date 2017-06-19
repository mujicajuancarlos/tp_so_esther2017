/*
 * memoryStruct.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MEMORYSTRUCT_H_
#define MEMORYSTRUCT_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include "configuration.h"

typedef struct memory_page {
	void* startAddress;
	int pid;
	int procPage;
	int globPage;
	bool isFree;
} memory_page;

typedef struct cache_entry {
	int pid;
	int procPage;
	int globPage;
} cache_entry;


typedef struct memory_struct {
	int socketServer;
	int socketClientKernel;
	pthread_mutex_t socketClientKernelMutex;
	t_list* listaCPUs;
	pthread_mutex_t listaCPUsMutex;
	Configuration* config;
	uint32_t memorySize;
	void* memory;
	t_list* referenceTable;
	t_list* cacheEntries;
} memory_struct;

#endif /* MEMORYSTRUCT_H_ */
