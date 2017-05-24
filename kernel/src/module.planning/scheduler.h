/*
 * scheduler.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_SCHEDULER_H_
#define MODULE_PLANNING_SCHEDULER_H_

#include "longTermScheduler.h"
#include "shortTermScheduler.h"
#include "processLifeCycle.h"


void initializeScheduler(kernel_struct* kernelStruct);

void destroyScheduler();

#endif /* MODULE_PLANNING_SCHEDULER_H_ */
