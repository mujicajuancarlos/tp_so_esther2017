/*
 * cpuResponse.h
 *
 *  Created on: 31/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPURESPONSE_H_
#define MODULE_CPU_CPURESPONSE_H_

#include <stdbool.h>
#include <parser/parser.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-process-exitCodes.h>
#include "../module.model/cpu.h"

void notifyUpdateSemaphoreStatus(CPU* cpu,bool hasError, bool shouldLock);

#endif /* MODULE_CPU_CPURESPONSE_H_ */
