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

typedef struct table_element {
	int pid;
	int procPage;
	int globPage;
} table_element;

typedef struct __attribute__((packed)) heap_meta_data {
	uint32_t size;
	bool isFree;
} heap_meta_data;

typedef struct memory_struct {
	int socketServer;
	int socketClientKernel;
	pthread_mutex_t socketClientKernelMutex;
	t_list* listaCPUs;
	pthread_mutex_t listaCPUsMutex;
	Configuration* config;
	uint32_t memorySize;
	uint32_t pageSize;
	void* memory;
	t_list* referenceTable;
} memory_struct;

#endif /* MEMORYSTRUCT_H_ */
