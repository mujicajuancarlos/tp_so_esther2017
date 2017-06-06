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
#include <unistd.h>
#include <commons/string.h>
#include <commons/collections/queue.h>
#include "../module.planning/shortTermScheduler.h"
#include "../module.model/processFinder.h"
#include <dc-commons/logger.h>
#include <dc-commons/utils.h>
#include <../dc-commons/protocol-kernel-console.h>
#include <../dc-commons/socket-message.h>
#include "../module.model/process.h"
#include "shortTermScheduler.h"
#include "longTermScheduler.h"



typedef struct {
	t_list* new;
	t_queue* ready;
	t_list* execute;
	t_queue* block;
	t_queue* exit;
} t_planningStates;



enum planningStates {
  new=1,
  ready=2,
  execute=3,
  block=4,
  ex=5
} enum_planning;  // Must end with a semicolon like a struct



void moveFromNewToReady(Process* process);
void moveFromExcecToReady(Process* process);
void moveFromExcecToExit(Process* process);
void moveFromExcecToBlock(Process* process);

void sendToREADY(Process* process);
Process* popToREADY();

void sendToEXEC(Process* process);
void removeFromEXEC(Process* process);

void sendToBLOCK(Process* process);
Process* popToBLOCK();

void sendToEXIT(Process* process);
Process* popToEXIT();

void sendToNEW(Process* process);
void removeFromNEW(Process* process);

t_planningStates* getStates();

void initializeProcessLifeCycle();
void destroyProcessLifeCycle();
void endProcessGeneric(Process* process);




#endif /* MODULE_PLANNING_PROCESSLIFECYCLE_H_ */
