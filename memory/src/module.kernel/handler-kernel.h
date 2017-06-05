/*
 * handler-kernel.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_HANDLER_KERNEL_H_
#define MODULE_KERNEL_HANDLER_KERNEL_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../module.model/kernel.h"
#include "kernelRequest.h"
#include "kernelResponse.h"

void handleKernel(kernel* kernel);

void handleKernelRequest(kernel* kernel, Package* package);

#endif /* MODULE_KERNEL_HANDLER_KERNEL_H_ */
