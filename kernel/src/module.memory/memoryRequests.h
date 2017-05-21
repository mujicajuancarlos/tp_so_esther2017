/*
 * memoryManager.h
 *
 *  Created on: 19/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MEMORY_MEMORYREQUESTS_H_
#define MODULE_MEMORY_MEMORYREQUESTS_H_

#include <stddef.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-errors.h>
#include <dc-commons/protocol-memory-kernel.h>
#include <dc-commons/serialization.h>
#include "../module.model/process.h"
#include "../kernelStruct.h"


void loadMemoryPageSize(kernel_struct* kernel_struct);

void reservePagesForNewProcess(Process* process, Package* package);

void sendSourceCodeForNewProcess(Process* process, Package* package);

#endif /* MODULE_MEMORY_MEMORYREQUESTS_H_ */