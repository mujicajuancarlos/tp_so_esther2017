/*
 * cpu.h
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_CPU_H_
#define HANDLER_CPU_H_

#include <dc-commons/package.h>
#include <dc-commons/logger.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>

#include "../kernelStruct.h"
#include "../module.model/cpu.h"
#include "cpuAdministrator.h"

void handleCPUs(kernel_struct *args);

void handleNewCPU(CPU* newCpu);

void handleCPURequest(CPU* cpu, Package* package);

#endif /* HANDLER_CPU_H_ */
