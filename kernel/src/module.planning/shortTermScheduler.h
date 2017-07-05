/*
 * shortTermScheduler.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_SHORTTERMSCHEDULER_H_
#define MODULE_PLANNING_SHORTTERMSCHEDULER_H_

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <dc-commons/logger.h>


void initializeshortTermScheduler();
void destroyshortTermScheduler();

void shortTermScheduler_lock();
void shortTermScheduler_unlock();

void shortTermSchedulerMutex_lock();
void shortTermSchedulerMutex_unlock();

bool isEnableShortTermScheduler();

void processInReady_wait();
void processInReady_signal();

#endif /* MODULE_PLANNING_SHORTTERMSCHEDULER_H_ */
