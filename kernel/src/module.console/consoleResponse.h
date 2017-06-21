/*
 * consoleResponse.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CONSOLE_CONSOLERESPONSE_H_
#define MODULE_CONSOLE_CONSOLERESPONSE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-server.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-kernel-console.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include "consoleRequests.h"
#include "../module.model/process.h"
#include "../kernelStruct.h"

void consoleResponseRepulseMessage(Process* newProcess);

void runProgramIsOK_response(Process* process);

void notifyEndProcess(Process* process);

void writeOnStdout(Process* process, t_data_FD_request* dataRequest);

#endif /* MODULE_CONSOLE_CONSOLERESPONSE_H_ */
