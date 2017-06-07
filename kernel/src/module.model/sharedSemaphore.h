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
#include <parser/parser.h>
#include <commons/collections/dictionary.h>
#include <dc-commons/logger.h>

void initializeSemaphores(char** semKeys, char** semValues);

bool executeBasicWait(t_nombre_semaforo semId);

void executeBasicSignal(t_nombre_semaforo semId);

#endif /* MODULE_PLANNING_SHAREDSEMAPHORE_H_ */
