/*
 * console.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <time.h>

//commoms
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

//commons death coders
#include <dc-commons/logger.h>
#include <dc-commons/package.h>
#include <dc-commons/socket-client.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-console.h>

#include "configuration.h"

typedef struct console_struct{
	t_list* listaProgramas;
	Configuration* config;
} console_struct;

typedef struct Program{
	int pid;
	struct timeval startDate;
	struct timeval endDate;
	int fd_client;
	pthread_t hilo;
	char* sourceCodePath;
} Program;

void initializeStruct(console_struct* args, Configuration* config);

#endif /* CONSOLE_H_ */
