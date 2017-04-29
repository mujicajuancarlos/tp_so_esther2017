/*
 * console.h
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_CONSOLE_H_
#define HANDLER_CONSOLE_H_

#include <commons/collections/list.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-console.h>

#include "../kernelStruct.h"
#include "../commonFunctions.h"

void handleConsolas(kernel_struct *args);
void handleConsoleRequest(int fileDescriptor, Package *package, kernel_struct *args);
void crearServerSocketParaConsola(kernel_struct* args);
void nuevaConsola(t_list* listaCPUs, int socketCPU_fd);

#endif /* HANDLER_CONSOLE_H_ */
