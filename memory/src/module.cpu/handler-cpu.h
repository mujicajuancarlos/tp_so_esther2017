/*
 * handler-cpu.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_HANDLER_CPU_H_
#define MODULE_CPU_HANDLER_CPU_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-cpu.h>
#include "../memoryClient.h"
#include "cpuResponse.h"
#include "cpu.h"

void handleCpu(memory_struct* memoryStruct, MemoryClient* newClient);

void handleCpuRequest(memory_struct* memoryStruct, CPU* cpu, Package* package);

void addCPU(CPU* cpu);

void removeCPU(CPU* cpu);

#endif /* MODULE_CPU_HANDLER_CPU_H_ */
