/*
 * cpuResponse.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPURESPONSE_H_
#define MODULE_CPU_CPURESPONSE_H_

#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-cpu.h>
#include <dc-commons/serialization.h>
#include "../memoryStruct.h"
#include "cpu.h"


void sendPageSizeToCPU(CPU* cpu, memory_struct* memoryStruct);

#endif /* MODULE_CPU_CPURESPONSE_H_ */
