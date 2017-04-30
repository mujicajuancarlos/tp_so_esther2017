/*
 * cpu.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include "../memoryStruct.h"
#include "../memoryClient.h"

//estructura para representar una CPU conectada a la memoria
typedef struct CPU {
	int fileDescriptor;	//file descriptor del socket del cpu
	memory_struct* memoryStruct;
} CPU;

CPU* createCPU(MemoryClient* newClient);

void destroyCPU(CPU* cpu);

#endif /* CPU_H_ */
