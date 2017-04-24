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
#include "console.h"
#include "program.h"
#include "handler-kernel.h"

int handleNewProgram(Program *program);

void addProgram(Program* program);

void removeProgram(Program* program);

#endif /* HANDLER_PROGRAM_H_ */
