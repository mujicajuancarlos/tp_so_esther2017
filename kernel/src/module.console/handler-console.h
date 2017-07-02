/*
 * console.h
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_CONSOLE_H_
#define HANDLER_CONSOLE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-kernel-console.h>
#include "consoleRequests.h"
#include "../module.model/process.h"
#include "../kernelStruct.h"

void handleConsolas(kernel_struct *args);

void handleNewProcess(Process* newProcess);

void handleConsoleRequestForProcess(Process* newProcess, Package* package);

#endif /* HANDLER_CONSOLE_H_ */
