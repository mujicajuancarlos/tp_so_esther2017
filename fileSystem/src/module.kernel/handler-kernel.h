/*
 * handler-kernel.h
 *
 *  Created on: 18/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_KERNEL_H_
#define HANDLER_KERNEL_H_

#include <dc-commons/package.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-fileSystem-kernel.h>
#include "../fileSystemStruct.h"
#include "kernelRequest.h"

void handleKernel(fileSystem_struct *fsStruct);

void handleKernelRequest(fileSystem_struct *fsStruct, Package *package);

#endif /* HANDLER_KERNEL_H_ */
