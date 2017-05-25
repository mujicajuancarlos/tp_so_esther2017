/*
 * handler-planning.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_HANDLER_PLANNING_H_
#define MODULE_PLANNING_HANDLER_PLANNING_H_

#include <dc-commons/protocol-kernel-cpu.h>
#include "../kernelStruct.h"
#include "../module.cpu/cpuAdministrator.h"
#include "scheduler.h"

void handlePlanning(kernel_struct *args);

#endif /* MODULE_PLANNING_HANDLER_PLANNING_H_ */
