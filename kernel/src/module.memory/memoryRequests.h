/*
 * memoryManager.h
 *
 *  Created on: 19/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MEMORY_MEMORYREQUESTS_H_
#define MODULE_MEMORY_MEMORYREQUESTS_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-process-exitCodes.h>
#include <dc-commons/protocol-memory-kernel.h>
#include <dc-commons/protocol-errors.h>
#include <dc-commons/serialization.h>
#include "../module.planning/processLifeCycle.h"
#include "../module.model/process.h"
#include "../kernelStruct.h"
#include "../module.console/consoleResponse.h"
#include "dynamicMemoryManager.h"

void initializeMemoryRequestMutex();
void destroyMemoryRequestMutex();
void memoryRequestMutex_lock();
void memoryRequestMutex_unlock();

void loadMemoryPageSize(kernel_struct* kernel_struct);

void reserveNewHeapPageForProcess(Process* process, int* status);

void reservePagesForNewProcess(Process* process, Package* package);

void freePageForProcess(Process* process, int pageNumber, int* status);

void notifyEndProcessToMemory(Process* process);

void sendSourceCodeForNewProcess(Process* process, Package* package);
void sendHeapMetadata(Process* process, heap_page* page,
		heap_metadata* metadata);
void readHeapMetadata(Process* process, heap_page* page,
		heap_metadata* metadata);

char* getDataFromPage(Process* process, int pageNumber, int offset, int size);

void saveDataOnMemory(Process* process, int startPage, u_int32_t offset,
		t_size length, char* buffer, bool* hasError);

void saveDataOnPage(Process* process, int pageNumber, int offset, int size,
		char* buffer, bool* hasError);

#endif /* MODULE_MEMORY_MEMORYREQUESTS_H_ */
