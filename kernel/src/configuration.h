/*
 * configuration.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef CONFIGURATION_CONFIGURATION_H_
#define CONFIGURATION_CONFIGURATION_H_

//import commoms library
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <dc-commons/config.h>
#include "module.model/sharedSemaphore.h"
#include "module.model/sharedVariables.h"

//configuration file keys
#define DEFAULT_CONFIG_PATH "../kernel.conf"
#define ECLIPSE_CONFIG_PATH "kernel.conf"
//process keys
#define PUERTO_PROG "PUERTO_PROG"
#define PUERTO_CPU "PUERTO_CPU"
#define IP_MEMORIA "IP_MEMORIA"
#define PUERTO_MEMORIA "PUERTO_MEMORIA"
#define IP_FS "IP_FS"
#define PUERTO_FS "PUERTO_FS"
#define QUANTUM "QUANTUM"
#define QUANTUM_SLEEP "QUANTUM_SLEEP"
#define ALGORITMO "ALGORITMO"
#define GRADO_MULTIPROG "GRADO_MULTIPROG"
#define SEM_IDS "SEM_IDS"
#define SEM_INIT "SEM_INIT"
#define SHARED_VARS "SHARED_VARS"
#define STACK_SIZE "STACK_SIZE"
//log keys
#define LOG_LEVEL "LOG_LEVEL"
#define LOG_FILE "LOG_FILE"
#define LOG_PROGRAM_NAME "LOG_PROGRAM_NAME"
#define LOG_PRINT_CONSOLE "LOG_PRINT_CONSOLE"

typedef struct Configuration {
	int puerto_program;
	int puerto_cpu;
	char* ip_memoria;
	int puerto_memoria;
	char* ip_fs;
	int puerto_fs;
	int quantum;
	int quantum_sleep;
	char* algoritmo;
	int grado_multiprog;
	int stack_size;
	char* log_level;
	char* log_file;
	char* log_program_name;
	bool log_print_console;
} Configuration;

Configuration* config;
char* config_file;

/**
 * Crea la configuracion y la devuelve
 */
Configuration* config_with(char *config_file);

/**
 * devuelve la configuracion
 */
Configuration* getConfiguration();

#endif /* CONFIGURATION_CONFIGURATION_H_ */
