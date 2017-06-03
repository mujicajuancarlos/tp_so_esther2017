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

void addNewPages (Package* package, memory_struct* memoryStruct);

void sendPageSizeToKernel(memory_struct* memoryStruct);

void saveData (Package* package, memory_struct* memoryStruct);

void readData (Package* package, memory_struct* memoryStruct);

void endProcess (memory_struct* memoryStruct, int pid);

#endif /* MODULE_KERNEL_KERNELREQUEST_H_ */
