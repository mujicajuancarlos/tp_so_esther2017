/*
 * printing-messages.h
 *
 *  Created on: 26/4/2017
 *      Author: utnso
 */

#ifndef PRINTING_MESSAGES_H_
#define PRINTING_MESSAGES_H_

#include <stdio.h>
#include <pthread.h>

#include "protocol-kernel-user.h"

void printNewLine(pthread_mutex_t stdoutMutex);

void printInvalidArguments(pthread_mutex_t stdoutMutex, char* command);

void printInvalidOptions(pthread_mutex_t stdoutMutex, char* command);

void printCommandsHelp(pthread_mutex_t stdoutMutex);

void printWelcome();

#endif /* PRINTING_MESSAGES_H_ */
