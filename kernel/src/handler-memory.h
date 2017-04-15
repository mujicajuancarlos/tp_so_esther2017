/*
 * handlerMemory.h
 *
 *  Created on: 14/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_MEMORY_H_
#define HANDLER_MEMORY_H_

#include <dc-commons/logger.h>
#include <dc-commons/socket-client.h>
#include <dc-commons/socket-message.h>

#include "kernel.h"

/**
 * @NAME: handleMemoria
 * @DESC: recive un puntero a la estructura de configuracion del kernel
 * Se encarga de establecer comunicacion con el proceso memoria
 * @PARAMS:
 * 		puntero a estructura kernel
 */
void handleMemoria(kernel_struct *args);

#endif /* HANDLER_MEMORY_H_ */
