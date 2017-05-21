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
#include <commons/collections/queue.h>
#include <dc-commons/logger.h>
#include "../module.model/process.h"


typedef struct {
	t_list* new;
	t_queue* ready;
	t_list* execute;
	t_queue* block;
	t_queue* exit;
} t_planningStates;

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

void initializeProcessLifeCycle();
void destroyProcessLifeCycle();

#endif /* MODULE_PLANNING_PROCESSLIFECYCLE_H_ */
