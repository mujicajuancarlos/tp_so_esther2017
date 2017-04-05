/*
 * configuration.h
 *
 *  Created on: 5/4/2017
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
#define DEFAULT_CONFIG_PATH "../config_consola.txt"
#define ECLIPSE_CONFIG_PATH "config_consola.txt"
#define IP_KERNEL "IP_KERNEL"
#define PUERTO_KERNEL "PUERTO_KERNEL"


typedef struct Configuration {
	int ip_kernel;
	int puerto_kernel;
} Configuration;

Configuration* config;
char* config_dir;
char* config_file_name;

#endif /* CONFIGURATION_H_ */
