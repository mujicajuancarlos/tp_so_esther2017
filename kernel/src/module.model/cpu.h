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

//estructura para representar una CPU conectada al kernel
typedef struct CPU {
	int fileDescriptor;	//file descriptor del socket del cpu
	int pid;	//PID del proceso que esta ejecutando actualmente
	int libre;	// 0:ocupado / 1:libre
} CPU;

CPU* createCPU(int socket);

void destroyCPU(CPU* cpu);

#endif /* CPU_H_ */
