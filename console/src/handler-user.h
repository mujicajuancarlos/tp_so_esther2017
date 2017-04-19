/*
 * handler-user.h
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_USER_H_
#define HANDLER_USER_H_

#include <stdio.h>
#include <stdlib.h>
#include "handler-program.h"
#include "protocol-console-user.h"

void handleUserRequest(console_struct* args);

void clearConsoleScreen();

void printWelcome();

#endif /* HANDLER_USER_H_ */
