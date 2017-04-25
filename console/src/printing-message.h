/*
 * printing-message.h
 *
 *  Created on: 25/4/2017
 *      Author: utnso
 */

#ifndef PRINTING_MESSAGE_H_
#define PRINTING_MESSAGE_H_

#include <commons/string.h>
#include <dc-commons/utils.h>
#include "protocol-console-user.h"

void printNewLine(pthread_mutex_t stdoutMutex);

void printFileNotFound(pthread_mutex_t stdoutMutex, char* filePath);

void printInvalidArguments(pthread_mutex_t stdoutMutex, char* command);

void printInvalidOptions(pthread_mutex_t stdoutMutex, char* command);

void printCommandsHelp(pthread_mutex_t stdoutMutex);

void printWelcome();

#endif /* PRINTING_MESSAGE_H_ */
