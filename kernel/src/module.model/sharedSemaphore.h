/*
 * sharedSemaphore.h
 *
 *  Created on: 6/6/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_SHAREDSEMAPHORE_H_
#define MODULE_PLANNING_SHAREDSEMAPHORE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <parser/parser.h>
#include <commons/collections/dictionary.h>
#include <dc-commons/logger.h>

#define SHARED_VAR_INIT_VALUE 0
#define UPDATE_SEM_SUCCESS 1
#define UPDATE_SEM_FAILURE -1

void initializeSemaphores(char** semKeys, char** semValues);

int executeBasicWait(t_nombre_semaforo semId, bool* shouldLock);

int executeBasicSignal(t_nombre_semaforo semId, bool* shouldUnlock);

#endif /* MODULE_PLANNING_SHAREDSEMAPHORE_H_ */
