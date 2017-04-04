/*
 * configuration.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//import standart library
#include <stdio.h>
#include <stdlib.h>

//import commoms library
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

//keys de configuracion
#define DEFAULT_CONFIG_PATH "../kernel.conf"
#define ECLIPSE_CONFIG_PATH "kernel.conf"
#define PUERTO_KERNEL "PUERTO_KERNEL"
#define LOG_LEVEL "LOG_LEVEL"
#define LOG_FILE "LOG_FILE"
#define LOG_PROGRAM_NAME "LOG_PROGRAM_NAME"
#define LOG_PRINT_CONSOLE "LOG_PRINT_CONSOLE"

typedef struct Configuration {
	int puerto;
	char* log_level;
	char* log_file;
	char* log_program_name;
	int log_print_console;
} Configuration;

Configuration* config;
char* config_dir;
char* config_file_name;

#endif /* CONFIGURATION_H_ */
