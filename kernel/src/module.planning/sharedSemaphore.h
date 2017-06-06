/*
 * sharedSemaphore.h
 *
 *  Created on: 6/6/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_SHAREDSEMAPHORE_H_
#define MODULE_PLANNING_SHAREDSEMAPHORE_H_

#include <stdbool.h>
#include <parser/parser.h>

bool executeBasicWait(t_nombre_semaforo semId);

void executeBasicSignal(t_nombre_semaforo semId);

#endif /* MODULE_PLANNING_SHAREDSEMAPHORE_H_ */
