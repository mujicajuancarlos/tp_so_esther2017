/*
 * cpuResponse.h
 *
 *  Created on: 31/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPURESPONSE_H_
#define MODULE_CPU_CPURESPONSE_H_

#include <parser/parser.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-process-exitCodes.h>
#include "../module.model/cpu.h"
#include "../module.model/process.h"
#include "../module.planning/processLifeCycle.h"
#include "../module.planning/sharedSemaphore.h"
#include "cpuAdministrator.h"


void executeWaitTo(CPU* cpu, Package* package);

void executeSignalTo(CPU* cpu, Package* package);

#endif /* MODULE_CPU_CPURESPONSE_H_ */
