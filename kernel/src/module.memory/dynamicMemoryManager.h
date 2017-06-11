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
#include "../kernel.h"
#include "heapMetadata.h"


#define MALLOC_MEMORY_SUCCES 0
#define FREE_MEMORY_SUCCES 0

int basicMallocMemory(Process* process, int mallocSize, t_puntero* pointer);

int basicFreeMemory(Process* process, t_puntero pointer);

heap_page* getHeapPageForProcess(int allocSize, Process* process, int* status);
heap_page* getHeapPageIntoListFor(int allocSize, t_list* heapList);
heap_page* createHeapPageFor(Process* process, int* status);
void validateMaxAllockSize(int allocSize, Process* process, int* status);
uint32_t getHeapFistPage(Process* process);
t_puntero logicalAddressToPointer(dir_memoria address, Process* process);
dir_memoria pointerToLogicalAddress(t_puntero pointer, Process* process);

#endif /* MODULE_MEMORY_DYNAMICMEMORYMANAGER_H_ */
