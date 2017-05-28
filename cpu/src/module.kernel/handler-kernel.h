/*
 * handler-kernel.h
 *
 *  Created on: 15/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_HANDLER_KERNEL_H_
#define MODULE_KERNEL_HANDLER_KERNEL_H_

#include <stddef.h>
#include <errno.h>

#include <dc-commons/package.h>
#include <dc-commons/logger.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-errors.h>
#include "kernelRequest.h"
#include "../cpuStruct.h"

void handleKernel(cpu_struct *args);

/**
 * Manejo un request del kernel
 */
void handleKernelRequest(cpu_struct *args, Package* package);


#endif /* MODULE_KERNEL_HANDLER_KERNEL_H_ */
