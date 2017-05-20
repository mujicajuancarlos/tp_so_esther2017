/*
 * memoryManager.h
 *
 *  Created on: 19/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MEMORY_MEMORYMANAGER_H_
#define MODULE_MEMORY_MEMORYMANAGER_H_

#include <stddef.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-kernel.h>
#include <dc-commons/serialization.h>
#include "../kernelStruct.h"


void loadMemoryPageSize(kernel_struct* kernel_struct);

#endif /* MODULE_MEMORY_MEMORYMANAGER_H_ */
