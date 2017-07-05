/*
 * kernelRequest.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_KERNEL_KERNELREQUEST_H_
#define MODULE_KERNEL_KERNELREQUEST_H_

#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../module.core/pageAdministrator.h"
#include "../module.model/kernel.h"
#include "../memoryStruct.h"

void startNewProcess(Package* package, kernel* kernel);

void addNewPages (Package* package, kernel* kernel);

void freeMemoryPage (Package* package, kernel* kernel);

void saveData (Package* package, kernel* kernel);

void readData(Package* package, kernel* kernel);

void endProcess (Package* package, kernel* kernel);

void kernelDisconnect (kernel* kernel);

#endif /* MODULE_KERNEL_KERNELREQUEST_H_ */
