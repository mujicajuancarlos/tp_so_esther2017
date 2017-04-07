/*
 * configuration.h
 *
 *  Created on: 6/4/2017
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
#define DEFAULT_CONFIG_PATH "../cpu.conf"
#define ECLIPSE_CONFIG_PATH "cpu.conf"
#define ip_memoria "ip_memoria"
#define puerto_memoria "puerto_memoria"

//log keys
#define LOG_LEVEL "LOG_LEVEL"
#define LOG_FILE "LOG_FILE"
#define LOG_PROGRAM_NAME "LOG_PROGRAM_NAME"
#define LOG_PRINT_CONSOLE "LOG_PRINT_CONSOLE"

typedef struct Configuration {
	int ip_memoria;
	int puerto_memoria;
	char* log_level;
	char* log_file;
	char* log_program_name;
	int log_print_console;
} Configuration;

Configuration* config;
char* config_dir;
char* config_file_name;

/**
 * Crea la configuracion y la devuelve
 */
Configuration* config_with(char *config_file);

/**
 * devuelve la configuracion
 */
Configuration* getConfiguration();

#endif /* CONFIGURATION_H_ */
