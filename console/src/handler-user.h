/*
 * handler-user.h
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_USER_H_
#define HANDLER_USER_H_

#include <stdbool.h>

#include <commons/string.h>
#include <dc-commons/utils.h>
#include <dc-commons/stdout-printer.h>

#include "handler-program.h"
#include "protocol-console-user.h"

void handleUserRequest(console_struct* args);

void handleCommand_start_program(console_struct* console, char** commands);

void handleCommand_info_program(console_struct* consoleStruct, char** commands);

void handleCommand_info_all_program(console_struct* consoleStruct, char** commands);

void handleCommand_info_by_pid_program(console_struct* consoleStruct, char** commands);

void handleCommand_end_program(console_struct* consoleStruct, char** commands);

void handleCommand_end_all_program(console_struct* consoleStruct, char** commands);

void handleCommand_end_by_pid_program(console_struct* consoleStruct, char** commands);

void printCommandsHelp();

#endif /* HANDLER_USER_H_ */
