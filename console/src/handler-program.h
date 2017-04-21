/*
 * handler-program.h
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_PROGRAM_H_
#define HANDLER_PROGRAM_H_

#include <stdlib.h>
#include <time.h>
#include <commons/process.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-client.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/utils.h>
#include <dc-commons/protocol-kernel-console.h>
#include "console.h"

int handleNewProgram(Program *program);

void handleKernelRequest(Program *program, Package* package);

Program* createNewProgram(console_struct* console, pthread_t *thread, char* sourceCodePath);

void destroyProgram(Program* program);

#endif /* HANDLER_PROGRAM_H_ */
