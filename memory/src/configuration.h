/* configuration_memory.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//import standart library
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

//import commoms library
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

//keys de configuracion
#define DEFAULT_CONFIG_PATH "../memory.conf"
#define ECLIPSE_CONFIG_PATH "memory.conf"
#define PUERTO "PUERTO"
#define MARCOS "MARCOS"
#define MARCO_SIZE "MARCO_SIZE"
#define ENTRADAS_CACHE "ENTRADAS_CACHE"
#define CACHE_X_PROC "CACHE_X_PROC"
#define REEMPLAZO_CACHE "REEMPLAZO_CACHE"
#define RETARDO_MEMORIA "RETARDO_MEMORIA"
#define TAMANIO_MEMORIA "TAMANIO_MEMORIA"

//log keys
#define LOG_LEVEL "LOG_LEVEL"
#define LOG_FILE "LOG_FILE"
#define LOG_PROGRAM_NAME "LOG_PROGRAM_NAME"
#define LOG_PRINT_CONSOLE "LOG_PRINT_CONSOLE"


typedef struct Configuration {
	int puerto;
	int marcos;
	int marco_size;
	int entradas_cache;
	int cache_x_proceso;
	char* reemplazo_cache;
	int retardo_memoria;
	char* log_level;
	char* log_file;
	char* log_program_name;
	bool log_print_console;
	int tamanio_memoria;
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
