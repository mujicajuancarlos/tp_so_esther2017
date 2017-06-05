/*
 * handler-memory.h
 *
 *  Created on: 27/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_CLIENTS_H_
#define HANDLER_CLIENTS_H_

#include <pthread.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-memory-identification.h>
#include <dc-commons/protocol-memory-cpu.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "module.cpu/handler-cpu.h"
#include "memoryStruct.h"
#include "module.model/cpu.h"
#include "module.model/kernel.h"
#include "module.model/memoryClient.h"
#include "module.kernel/handler-kernel.h"


void handleClients(memory_struct* memoryStruct);

void handleNewClient(MemoryClient* newClient);

#endif /* HANDLER_CLIENTS_H_ */
