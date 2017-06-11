/*
 * cpuRequest.h
 *
 *  Created on: 23/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPUREQUEST_H_
#define MODULE_CPU_CPUREQUEST_H_

#include <parser/parser.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-process-exitCodes.h>

#include "../module.memory/dynamicMemoryManager.h"
#include "../module.model/cpu.h"
#include "../module.model/process.h"
#include "../module.planning/processLifeCycle.h"
#include "../module.planning/scheduler.h"
#include "cpuAdministrator.h"
#include "cpuResponse.h"


void startProcessExecution(Process* selectedProcess, CPU* selectedCPU);

void continueCurrentExcecution(CPU* cpu);
void contextSwitch(CPU* cpu);
void contextSwitchForBlocked(CPU* cpu, t_nombre_semaforo semId);

void executeWaitTo(CPU* cpu, Package* package);
void executeSignalTo(CPU* cpu, Package* package);

void executeSetSharedVar(CPU* cpu, Package* package);
void executeGetSharedVar(CPU* cpu, Package* package);

void executeMallocMemoryTo(CPU* cpu, Package* package);
void executeFreeMemoryTo(CPU* cpu, Package* package);


void resolveRequest_endInstruction(CPU* cpu, Package* package);

void resolveRequest_programFinished(CPU* cpu, Package* package);

void resolveRequest_cpuDisconnected(CPU* cpu, Package* package);

void resolveRequest_sharedVarOperation(CPU* cpu, Package* package);

void resolveRequest_dynamicMemoryOperation(CPU* cpu, Package* package);

void resolveRequest_fileSystemOperation(CPU* cpu, Package* package);

void resolveRequest_updateSemaphore(CPU* cpu, Package* package);

void resolveRequest_executionError(CPU* cpu, Package* package);

#endif /* MODULE_CPU_CPUREQUEST_H_ */
