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
#include "kernelStruct.h"
#include "connection.h"
#include "printing-messages.h"


/**
 * genero los sockets server y clientes para conectarme con los otros procesos
 */
void createSockets(kernel_struct* kernelStruct);

/**
 * inicializo la estructura
 */
void initializeStruct(kernel_struct* args, Configuration* config);

#endif /* KERNEL_H_ */
