/*
 * cpuRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_CPU_CPUREQUEST_H_
#define MODULE_CPU_CPUREQUEST_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-memory-cpu.h>
#include "../memoryStruct.h"

void saveData (Package* package, memory_struct* memoryStruct);

void readData (Package* package, memory_struct* memoryStruct);

#endif /* MODULE_CPU_CPUREQUEST_H_ */
