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

#include "configuration.h"

typedef struct {
	t_list* listaProgramas;
	Configuration* config;
} console_struct;

typedef struct {
	int pid;
	time_t startDate;
	time_t endDate;
	int fd_client;
	char* sourceCodePath;
	console_struct* console;
} Program;

void initializeStruct(console_struct* args, Configuration* config);

#endif /* CONSOLE_H_ */
