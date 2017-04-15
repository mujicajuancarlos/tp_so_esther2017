/*
 * handlerFileSystem.h
 *
 *  Created on: 15/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_FILESYSTEM_H_
#define HANDLER_FILESYSTEM_H_

#include <dc-commons/logger.h>
#include <dc-commons/socket-client.h>
#include <dc-commons/socket-message.h>

#include "kernel.h"

/**
 * @NAME: handleFileSystem
 * @DESC: recibe un puntero a la estructura de configuracion del kernel
 * Se encarga de establecer comunicacion con el proceso filesystem
 * @PARAMS:
 * 		puntero a estructura kernel
 */
void handleFileSystem(kernel_struct *args);

#endif /* HANDLER_FileSystem_H_ */
