/*
 * cpu-core.h
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_CPU_CORE_H_
#define MODULE_CORE_CPU_CORE_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include "../module.memory/memoryRequest.h"
#include "../cpuStruct.h"


#define FLAG_OK 0
#define FLAG_STACKOVERFLOW -1
#define FLAG_SEGMENTATION_FAULT -2

void setPageSize(int value);

void loadPCB(PCB* newPCB);

void ansisopExecuteInstruccion(cpu_struct* cpuStruct);

void unloadPCB(cpu_struct* cpuStruct);

char* getNextInstruction(cpu_struct* cpuStruct);

char* getDataFromMemory(cpu_struct* cpuStruct, t_puntero_instruccion offset, t_size length);

#endif /* MODULE_CORE_CPU_CORE_H_ */
