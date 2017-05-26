/*
 * cpuRequest.h
 *
 *  Created on: 23/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPUREQUEST_H_
#define MODULE_CPU_CPUREQUEST_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include "../module.model/cpu.h"
#include "../module.model/process.h"
#include "../module.planning/processLifeCycle.h"


void startProcessExecution(Process* selectedProcess, CPU* selectedCPU);

#endif /* MODULE_CPU_CPUREQUEST_H_ */
