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
#include "../module.ansisop/primitiveFunctions.h"
#include "../module.ansisop/primitiveKernelFunctions.h"
#include "../module.memory/memoryRequest.h"
#include "../module.kernel/kernelResponse.h"
#include "../cpuStruct.h"
#include "error-flags.h"

PCB* getPCB();

void setErrorFlag(int error);

int getErrorFlag();

void setPageSize(int value);

void loadPCB(PCB* newPCB);

void ansisopExecuteInstruccion(cpu_struct* cpuStruct);

void unloadPCB(cpu_struct* cpuStruct);

char* getNextInstruction(cpu_struct* cpuStruct);

char* getDataFromMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset, t_size length);

void saveDataOnMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset, t_size length, char* buffer);

#endif /* MODULE_CORE_CPU_CORE_H_ */
