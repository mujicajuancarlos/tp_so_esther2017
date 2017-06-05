/*
 * cpuRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPUREQUEST_H_
#define MODULE_CPU_CPUREQUEST_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-memory-cpu.h>
#include "../module.model/cpu.h"
#include "../module.core/pageAdministrator.h"

void cpuSaveData(CPU* cpu, Package* package);

void cpuReadData(CPU* cpu, Package* package);

#endif /* MODULE_CPU_CPUREQUEST_H_ */
