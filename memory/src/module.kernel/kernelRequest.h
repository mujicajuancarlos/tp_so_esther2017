/*
 * kernelRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELREQUEST_H_
#define MODULE_KERNEL_KERNELREQUEST_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../memoryStruct.h"

void startNewProcess(Package* package, memory_struct* memoryStruct);

void startNewProcessTest (int processId, int stackSize, int sourceCodeSize, memory_struct* memoryStruct);

void sendPageSize(memory_struct* memoryStruct);

#endif /* MODULE_KERNEL_KERNELREQUEST_H_ */
