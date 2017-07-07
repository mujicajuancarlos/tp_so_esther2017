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
#include "memoryRequests.h"
#include "heapMetadata.h"


#define MALLOC_MEMORY_SUCCES 0
#define FREE_MEMORY_SUCCES 0

int basicMallocMemory(Process* process, int mallocSize, t_puntero* pointer);

int basicFreeMemory(Process* process, t_puntero pointer);

void executeGarbageCollectorOn(heap_page* page, Process* process, int* status);
void removeGarbagePages(Process* process);

void resolveFragmentation(Process* process, heap_page* page);

void saveAlloc(int allocSize, heap_metadata* metadata, int index, heap_page* page);

heap_page* getAvailableHeapPageForProcess(int allocSize, Process* process, int* status);
heap_page* getAvailableHeapPageIntoListFor(int allocSize, t_list* heapList);
heap_page* createHeapPageFor(Process* process, int* status);
void createHeapMetadataFor(heap_page* page, heap_metadata* previous, int previousIndex);

void validateMaxAllockSize(int allocSize, Process* process, int* status);

int getNextHeapPageNumber(t_list* pageList);
uint32_t getHeapFistPageNumber(Process* process);

t_puntero logicalAddressToPointer(dir_memoria address, Process* process);
dir_memoria pointerToMemoryLogicalAddress(t_puntero pointer, Process* process);
dir_memoria pointerToHeapLogicalAddress(t_puntero pointer, Process* process);

#endif /* MODULE_MEMORY_DYNAMICMEMORYMANAGER_H_ */
