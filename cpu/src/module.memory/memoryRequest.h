/*
 * memoryRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_MEMORY_MEMORYREQUEST_H_
#define MODULE_MEMORY_MEMORYREQUEST_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-errors.h>
#include <dc-commons/protocol-memory-cpu.h>
#include "../cpuStruct.h"
#include "../module.core/cpu-core.h"

void loadMemoryPageSize(cpu_struct* cpuStruct);

char* getDataFromPage(cpu_struct* cpuStruct, int pageNumber, int offset, int size);

#endif /* MODULE_MEMORY_MEMORYREQUEST_H_ */
