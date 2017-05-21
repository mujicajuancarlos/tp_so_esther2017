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
	t_queue* new;
	t_queue* ready;
	t_list* execute;
	t_queue* block;
	t_queue* exit;
} t_planningStates;

void sendToREADY(Process* process);
void sendToEXEC(Process* process);
void sendToBLOCK(Process* process);
void sendToEXIT(Process* process);
void sendToNEW(Process* process);

void initializeProcessLifeCycle();
void destroyProcessLifeCycle();

#endif /* MODULE_PLANNING_PROCESSLIFECYCLE_H_ */
