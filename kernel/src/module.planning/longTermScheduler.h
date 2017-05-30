/*
 * multiprogramming.h
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_LONGTERMSCHEDULER_H_
#define MODULE_PLANNING_LONGTERMSCHEDULER_H_

#include <semaphore.h>
#include <pthread.h>
#include <dc-commons/logger.h>
#include "../kernelStruct.h"

#define MAX_MULTIPROGRAMMING_LVL 1000

void initializeMultiprogrammingLevel(kernel_struct* kernel_struct);

void destroyMultiprogrammingLevel();

void updateMultiprogrammingLevel(kernel_struct* kernel_struct, int newLevel);

void incrementMultiprogrammingLevel(int number);

void decrementMultiprogrammingLevel(int* number);

void _incrementMultiprogrammingLevel();

void _decrementMultiprogrammingLevel();

#endif /* MODULE_PLANNING_LONGTERMSCHEDULER_H_ */
