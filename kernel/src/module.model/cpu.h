/*
 * cpu.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include <stdbool.h>
#include <stdlib.h>
#include "../kernelStruct.h"
#include "process.h"

//estructura para representar una CPU conectada al kernel
typedef struct CPU {
	int fileDescriptor;	//file descriptor del socket del cpu
	Process* process;	//proceso que esta ejecutando actualmente
	bool libre;	// 0:ocupado / 1:libre
	kernel_struct* kernelStruct;
} CPU;

CPU* createCPU(int socket, kernel_struct* kernelStruct);

void destroyCPU(CPU* cpu);

#endif /* CPU_H_ */
