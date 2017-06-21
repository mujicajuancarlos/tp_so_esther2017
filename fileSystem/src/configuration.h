/*
 * configuration.h
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

//commoms
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include <dc-commons/config.h>


//configuration file keys
#define DEFAULT_CONFIG_PATH "../fileSystem.conf"
#define ECLIPSE_CONFIG_PATH "fileSystem.conf"
//process keys
#define TAMANIO_BLOQUES "TAMANIO_BLOQUES"
#define CANTIDAD_BLOQUES "CANTIDAD_BLOQUES"
#define MAGIC_NUMBER "MAGIC_NUMBER"
#define TAMANIO "TAMANIO"
#define BLOQUES "BLOQUES"
#define PUERTO "PUERTO"
#define PUNTO_MONTAJE "PUNTO_MONTAJE"
//log keys
#define LOG_LEVEL "LOG_LEVEL"
#define LOG_FILE "LOG_FILE"
#define LOG_PROGRAM_NAME "LOG_PROGRAM_NAME"
#define LOG_PRINT_CONSOLE "LOG_PRINT_CONSOLE"

typedef struct Configuration {
	int puerto;
	char* punto_montaje;
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

#endif /* CONFIGURATION_H_ */
