/*
 * scheduler.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_SCHEDULER_H_
#define MODULE_PLANNING_SCHEDULER_H_

#include <string.h>
#include "../module.model/cpu.h"
#include "../module.cpu/cpuRequest.h"
#include "longTermScheduler.h"
#include "shortTermScheduler.h"
#include "processLifeCycle.h"

#define ROUND_ROBIN 1
#define FIFO 2

void initializeScheduler(kernel_struct* kernelStruct);

void destroyScheduler();

int getAlgorithmIndex(char* name);

void execute_RoundRobin_scheduling(kernel_struct *kernel_struct, CPU* selectedCPU);

void execute_FIFO_scheduling(kernel_struct *kernel_struct, CPU* selectedCPU);

Process* getReadyProcess_with_FIFO();

#endif /* MODULE_PLANNING_SCHEDULER_H_ */
