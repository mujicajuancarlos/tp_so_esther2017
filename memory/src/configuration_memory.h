/*
 * configuration_memory.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef CONFIGURATION_MEMORY_H_
#define CONFIGURATION_MEMORY_H_

//import standart library
#include <stdio.h>
#include <stdlib.h>

//import commoms library
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

//keys de configuracion
#define DEFAULT_CONFIG_PATH "../config_memory.txt"
#define ECLIPSE_CONFIG_PATH "config_memory.txt"
#define PUERTO "PUERTO"
#define MARCOS "MARCOS"
#define MARCO_SIZE "MARCO_SIZE"
#define ENTRADAS_CACHE "ENTRADAS_CACHE"
#define CACHE_X_PROC "CACHE_X_PROC"
#define REEMPLAZO_CACHE "REEMPLAZO_CACHE"
#define RETARDO_MEMORIA "RETARDO_MEMORIA"

typedef struct Configuration {
	int puerto;
	int marcos;
	int marco_size;
	int entradas_cache;
	int cache_x_proceso;
	char* reemplazo_cache;
	int retardo_memoria;
} Configuration;

Configuration* config;
char* config_dir;
char* config_file_name;


#endif /* CONFIGURATION_MEMORY_H_ */
