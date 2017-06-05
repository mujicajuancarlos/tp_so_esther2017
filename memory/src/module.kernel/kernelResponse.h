/*
 * kernelResponse.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELRESPONSE_H_
#define MODULE_KERNEL_KERNELRESPONSE_H_

#include <stddef.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/protocol-memory-kernel.h>
#include <dc-commons/socket-message.h>
#include "../module.model/kernel.h"

void sendPageSizeToKernel(kernel* kernel);

#endif /* MODULE_KERNEL_KERNELRESPONSE_H_ */
