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
#include "handler-program.h"
#include "protocol-console-user.h"

void handleUserRequest(console_struct* args);

void handleCommand_start_program(console_struct* console, char** commands, bool *shouldCompareCommand);

void printFileNotFound(char* filePath);

void printInvalidArguments(char* command);

void printInvalidOptions(char* command);

void printCommandsHelp();

void printWelcome();

#endif /* HANDLER_USER_H_ */
