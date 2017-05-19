/*
 * kernelRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELREQUEST_H_
#define MODULE_KERNEL_KERNELREQUEST_H_

#include <dc-commons/package.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../memoryStruct.h"

void startNewProcess(Package* package, memory_struct* memoryStruct);

void startNewProcessTest (int processId, int stackSize, int sourceCodeSize, memory_struct* memoryStruct);

void assignNewPages (memory_struct* memoryStruct, int processId, int pages);

void processWrite (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size);

void processRead (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size);

void memoryDump (); // NO DEBERÍA ESTAR ACA PORQUE ES UNA FUNCION DE LA CONSOLA DE LA MEMORIA

void endProcess (memory_struct *memoryStruct, int processId, int processPage);

#endif /* MODULE_KERNEL_KERNELREQUEST_H_ */
