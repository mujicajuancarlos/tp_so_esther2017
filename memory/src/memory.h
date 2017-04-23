/*
 * memory.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef MEMORY_H_
#define MEMORY_H_


#include <pthread.h>
//commoms
#include <commons/collections/list.h>
#include <commons/string.h>

//commons DeathCoders
#include <dc-commons/config.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-cpu.h>
#include <dc-commons/protocol-memory-kernel.h>

#include "configuration.h"
#include "Packages/packagesKernel.h"
#include "Packages/packagesCPU.h"

//estructura princial del cpu
typedef struct memory_struct {
	int socketServer;
	int socketClientKernel;
	t_list* listaCPUs;
	Configuration* config;
} memory_struct;

/**
 * Inicializacion de la estructura global
 */
void initializeStruct(memory_struct* args, Configuration* config);

#endif /* MEMORY_H_ */
