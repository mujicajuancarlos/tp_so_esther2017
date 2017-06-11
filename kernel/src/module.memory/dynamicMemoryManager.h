/*
 * dynamicMemoryCore.h
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#ifndef MODULE_MEMORY_DYNAMICMEMORYMANAGER_H_
#define MODULE_MEMORY_DYNAMICMEMORYMANAGER_H_

#include <parser/parser.h>
#include <dc-commons/protocol-process-exitCodes.h>
#include "../module.model/process.h"
#include "heapMetadata.h"


#define MALLOC_MEMORY_SUCCES 0
#define FREE_MEMORY_SUCCES 0

int basicMallocMemory(Process* process, int mallocSize, t_puntero* pointer);

int basicFreeMemory(Process* process, t_puntero pointer);

#endif /* MODULE_MEMORY_DYNAMICMEMORYMANAGER_H_ */
