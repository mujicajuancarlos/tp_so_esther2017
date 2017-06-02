/*
 * kernelResponse.h
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELRESPONSE_H_
#define MODULE_KERNEL_KERNELRESPONSE_H_

#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-errors.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-process-exitCodes.h>
#include "../module.core/error-flags.h"
#include "../module.core/cpu-core.h"

void reportEndInstruction(cpu_struct* cpuStruct);

void reportEndProcess(cpu_struct* cpuStruct);

void reportContextSwich(cpu_struct* cpuStruct);

void sendPCB(cpu_struct* cpuStruct, int messageCode);

void reportExcecutionError(cpu_struct* cpuStruct, int errorFlag);

int getSharedVarriableValue(cpu_struct* cpuStruct, char* name);

int setSharedVarriableValue(cpu_struct* cpuStruct, char* name, int value);

#endif /* MODULE_KERNEL_KERNELRESPONSE_H_ */
