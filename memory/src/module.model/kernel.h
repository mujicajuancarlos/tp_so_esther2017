/*
 * kernel.h
 *
 *  Created on: 4/6/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_KERNEL_H_
#define MODULE_MODEL_KERNEL_H_

#include "../memoryStruct.h"
#include "memoryClient.h"

//estructura para representar un kernel conectado
typedef struct {
	int fileDescriptor;	//file descriptor del socket del cpu
	memory_struct* memoryStruct;
} kernel;

kernel* createKernel(MemoryClient* newClient);

void destroyKernel(kernel* cpu);

#endif /* MODULE_MODEL_KERNEL_H_ */
