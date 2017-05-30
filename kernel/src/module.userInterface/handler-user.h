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
#include "../module.planning/processLifeCycle.h"
#include "../module.planning/longTermScheduler.h"
#include "../module.model/processFinder.h"
#include "../kernelStruct.h"
#include "../module.model/process.h"
#include "protocol-kernel-user.h"

void handleUserRequest(kernel_struct* kernelStruct);

void handleCommand_info_program(kernel_struct* kernelStruct, char** commands);

void handleCommand_info_all_process(kernel_struct* kernelStruct,
		char** commands);

void handleCommand_info_by_state_process(kernel_struct* kernelStruct,
		char** commands);

void handleCommand_info_by_pid_process(kernel_struct* kernelStruct,
		char** commands);

void handleCommand_set_new_multiprogramming_level(kernel_struct* kernelStruct,
		char** commands);

void handleCommand_end_program(kernel_struct* kernelStruct, char** commands);

void handleCommand_end_all_program(kernel_struct* kernelStruct, char** commands);

void handleCommand_end_by_pid_program(kernel_struct* kernelStruct,
		char** commands);

void printCommandsHelp();

#endif /* HANDLER_USER_C_ */
