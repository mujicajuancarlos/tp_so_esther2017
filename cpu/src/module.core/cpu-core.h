/*
 * cpu-core.h
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_CPU_CORE_H_
#define MODULE_CORE_CPU_CORE_H_

#include <dc-commons/logger.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include "../module.ansisop/primitiveFunctions.h"
#include "../module.ansisop/primitiveKernelFunctions.h"
#include "../module.memory/memoryRequest.h"
#include "../module.kernel/kernelResponse.h"
#include "error-flags.h"

void initExecutionMutex();
void destroyExecutionMutex();

PCB* getPCB();
void loadPCB(PCB* newPCB);
void unloadPCB();

void setStackPageSize(int size);

void setErrorFlag(int error);

int getErrorFlag();

void setPageSize(int value);

bool isFinishedProcess();

void validateStackOverflow(size_t requiredSize);

t_stack_index* getCurrentContext();

t_puntero logicalAddressToPointer(dir_memoria* dir);

void finalize_CPU_process();

void ansisopExecuteInstruccion(cpu_struct* cpuStruct);

char* getNextInstruction(cpu_struct* cpuStruct);

char* getDataFromMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset, t_size length);

void saveDataOnMemory(cpu_struct* cpuStruct, int startPage, u_int32_t offset, t_size length, char* buffer);

t_variable* createVariableForCurrentPCB(t_nombre_variable name);

t_variable* createArgumentForCurrentPCB(t_nombre_variable name);

#endif /* MODULE_CORE_CPU_CORE_H_ */
