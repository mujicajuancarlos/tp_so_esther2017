/*
 * configuration.c
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#include "configuration.h"

Configuration* config_with(char *config_file) {

	config_file = DEFAULT_CONFIG_PATH;

	Configuration* config = malloc(sizeof(Configuration));

	t_config* nConfig = config_create(
			config_file ? config_file : DEFAULT_CONFIG_PATH);
	if (nConfig == NULL) {
		//para debuggear desde eclipse
		nConfig = config_create(ECLIPSE_CONFIG_PATH);
		if (nConfig == NULL) {
			printf("No se encontro el archivo de configuracion.\n");
			exit(1);
		}
	}

	config->tamanio_bloques = config_get_int_value(nConfig, TAMANIO_BLOQUES);
	config->cantidad_bloques = config_get_int_value(nConfig, CANTIDAD_BLOQUES);
	config->magic_number = strdup(config_get_string_value(nConfig, MAGIC_NUMBER));
	//	config->bloques
	config->tamanio = config_get_int_value(nConfig, TAMANIO);
	config->puerto = config_get_int_value(nConfig, PUERTO);
	config->punto_montaje = strdup(config_get_string_value(nConfig, PUNTO_MONTAJE));
	//configuracion de log
	config->log_level = strdup(config_get_string_value(nConfig, LOG_LEVEL));
	config->log_file = strdup(config_get_string_value(nConfig, LOG_FILE));
	config->log_program_name = strdup(
			config_get_string_value(nConfig, LOG_PROGRAM_NAME));
	config->log_print_console = config_get_int_value(nConfig,
	LOG_PRINT_CONSOLE);

	config_destroy(nConfig);

	return config;
}

Configuration* getConfiguration() {
	return config;
}

