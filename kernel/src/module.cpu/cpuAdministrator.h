/*
 * cpuAdministrator.h
 *
 *  Created on: 23/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPUADMINISTRATOR_H_
#define MODULE_CPU_CPUADMINISTRATOR_H_

#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include "../module.model/cpu.h"

void initializeCpuAdministrator();

void destroyCpuAdministrator();

void markFreeCPU(CPU* cpu);

CPU* searchAndMarkBusyCPU();

void addCPU(CPU* cpu);

void removeCPU(CPU* cpu);


#endif /* MODULE_CPU_CPUADMINISTRATOR_H_ */
