/*
 * handler-user.c
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_USER_C_
#define HANDLER_USER_C_

#include <stdbool.h>
#include <pthread.h>
#include <dc-commons/stdout-printer.h>
#include <dc-commons/utils.h>
#include "../memoryStruct.h"
#include "protocol-memory-user.h"

void handleUserRequest(memory_struct* kernelStruct);

void handleCommand_dump(memory_struct* kernelStruct, char** commands);

void handleCommand_dump_all(memory_struct* memoryStruct, char** commands);

void handleCommand_dump_page(memory_struct* kernelStruct,
		char** commands);

void printCommandsHelp();

#endif /* HANDLER_USER_C_ */
