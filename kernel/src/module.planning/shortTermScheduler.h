/*
 * shortTermScheduler.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_SHORTTERMSCHEDULER_H_
#define MODULE_PLANNING_SHORTTERMSCHEDULER_H_

#include <stddef.h>
#include <pthread.h>
#include <dc-commons/logger.h>


void initializeshortTermScheduler();

void destroyshortTermScheduler();

void shortTermScheduler_lock();

void shortTermScheduler_unlock();

#endif /* MODULE_PLANNING_SHORTTERMSCHEDULER_H_ */
