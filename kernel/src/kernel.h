/*
 * kernel.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef KERNEL_H_
#define KERNEL_H_


#include <commons/config.h>
#include <dc-commons/logger.h>

#include "module.cpu/handler-cpu.h"
#include "module.model/process.h"
#include "module.console/handler-console.h"
#include "module.planning/handler-planning.h"
#include "module.userInterface/handler-user.h"

#include "kernelStruct.h"
#include "connection.h"


/**
 * genero los sockets server y clientes para conectarme con los otros procesos
 */
void createSockets(kernel_struct* kernelStruct);

/**
 * inicializo la estructura
 */
void initializeStruct(kernel_struct* args, Configuration* config);

#endif /* KERNEL_H_ */
