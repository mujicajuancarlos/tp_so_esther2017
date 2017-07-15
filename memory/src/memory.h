/*
 * memory.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include <pthread.h>
#include <dc-commons/logger.h>
#include "handler-clients.h"
#include "module.userInterface/handler-user.h"
#include "module.core/pageAdministrator.h"
#include "memoryStruct.h"

//estructura princial del cpu


/**
 * Inicializacion de la estructura global
 */
void createMemoryServerSocket(memory_struct* memoryStruct);

void initializeStruct(memory_struct* memoryStruct, Configuration* config);

#endif /* MEMORY_H_ */
