/*
 * kernelRequest.h
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELREQUEST_H_
#define MODULE_KERNEL_KERNELREQUEST_H_

#include <dc-commons/package.h>
#include <dc-commons/logger.h>
#include "../cpuStruct.h"
#include "../module.core/cpu-core.h"

void executeNewPcb(cpu_struct *cpuStruct, Package *package);

void continueExecution(cpu_struct *cpuStruct, Package *package);

void contextSwitch(cpu_struct *cpuStruct, Package *package);

#endif /* MODULE_KERNEL_KERNELREQUEST_H_ */
