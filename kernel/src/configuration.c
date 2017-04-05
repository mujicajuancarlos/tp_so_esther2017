/*
 * configuration.c
 *
 *  Created on: 3/4/2017
 *      Author: utnso
 */

#include "configuration.h"

Configuration* config_with(char *config_file) {

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

	config->puerto_program = config_get_int_value(nConfig, PUERTO_PROG);
	config->puerto_cpu = config_get_int_value(nConfig, PUERTO_CPU);
	config->ip_memoria = strdup(config_get_string_value(nConfig, IP_MEMORIA));
	config->puerto_memoria = config_get_int_value(nConfig, PUERTO_MEMORIA);
	config->ip_fs = strdup(config_get_string_value(nConfig, IP_FS));
	config->puerto_fs = config_get_int_value(nConfig, PUERTO_FS);
	config->quantum = config_get_int_value(nConfig, QUANTUM);
	config->quantum_sleep = config_get_int_value(nConfig, QUANTUM_SLEEP);
	config->algoritmo = strdup(config_get_string_value(nConfig, ALGORITMO));
	config->grado_multiprog = config_get_int_value(nConfig, GRADO_MULTIPROG);
//	config->sem_ids
//	config->sem_init
//	config->shared_vars
	config->stack_size = config_get_int_value(nConfig, STACK_SIZE);

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

