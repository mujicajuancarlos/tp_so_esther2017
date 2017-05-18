/*
 * console-request.h
 *
 *  Created on: 4/5/2017
 *      Author: utnso
 */

#ifndef MODULE_CONSOLE_CONSOLEREQUESTS_H_
#define MODULE_CONSOLE_CONSOLEREQUESTS_H_

#include <stdbool.h>
#include <stdlib.h>
#include <parser/metadata_program.h>
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/serialization.h>
#include <dc-commons/protocol-kernel-console.h>
#include <dc-commons/protocol-kernel-cpu.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../module.model/process.h"

void startNewProcess(Process* process, Package* package);

#endif /* MODULE_CONSOLE_CONSOLEREQUESTS_H_ */
