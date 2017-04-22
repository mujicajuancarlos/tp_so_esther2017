/*
 * handler-kernel.h
 *
 *  Created on: 18/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_KERNEL_H_
#define HANDLER_KERNEL_H_

#include <stddef.h>
#include <errno.h>

#include "fileSystem.h"

#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-fileSystem-kernel.h>

void handleKernel(fileSystem_struct *args);

void handleKernelRequest(fileSystem_struct *args, Package *package);

void comunicacionConKernel(fileSystem_struct *args);

void packagesSenderKernel(fileSystem_struct *args, int code);

void saludoAKernel(int sck);

void exceptionTo(int destinationfd, char *message);

#endif /* HANDLER_KERNEL_H_ */
