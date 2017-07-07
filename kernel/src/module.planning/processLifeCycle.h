/*
 * processLifeCycle.h
 *
 *  Created on: 5/5/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_PROCESSLIFECYCLE_H_
#define MODULE_PLANNING_PROCESSLIFECYCLE_H_

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <commons/string.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <dc-commons/logger.h>
#include <dc-commons/utils.h>
#include <dc-commons/protocol-process-exitCodes.h>
#include "../module.console/consoleResponse.h"
#include "../module.planning/shortTermScheduler.h"
#include "../module.model/processFinder.h"
#include "../module.model/process.h"
#include "../module.model/sharedSemaphore.h"
#include "shortTermScheduler.h"
#include "longTermScheduler.h"
#include "stateConverter.h"

typedef struct {
	t_queue* new;
	t_queue* ready;
	t_list* execute;
	t_list* block;
	t_queue* exit;
} t_planningStates;

//move to ready
void moveFromNewToReady(Process* process);
void moveFromExcecToReady(Process* process);
void moveFromBlockToReady(Process* process);
//move to exit from execute
void moveFromExcecToExit_withError(Process* process, int statusCode);
void moveFromExcecToExit_withoutError(Process* process);
void moveFromExcecToExit(Process* process);
//move to exit
void moveFromNewToExit(Process* process);
void moveFromReadyToExit(Process* process);
void moveFromBlockToExit(Process* process);
void removeFromBlockQueues(Process* process);
//move to block
void moveFromExcecToBlock(Process* process);

void sendToREADY(Process* process);
Process* popToREADY();

void sendToEXEC(Process* process);
void removeFromEXEC(Process* process);

void sendToBLOCK(Process* process);
void removeFromBLOCK(Process* process);
void initializeQueueBySemaphore(char** semKeys);
void notifyUnlockedProcessFor(char* semKey);
void notifyLockedProcessFor(char* semKey, Process* process);

void sendToEXIT(Process* process);
Process* popToEXIT();

void sendToNEW(Process* process);
Process* popToNEW();
Process* peekToNEW();
void removeFromREADY(Process* process);
void removeFromNEW(Process* process);

t_planningStates* getStates();

void initializeProcessLifeCycle();
void destroyProcessLifeCycle();

void basicForceQuitProcess(Process* process, int stateCode);


#endif /* MODULE_PLANNING_PROCESSLIFECYCLE_H_ */
