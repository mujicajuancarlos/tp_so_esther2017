/*
 * configuration_memory.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "configuration.h"

Configuration* config_with(char *config_file) {

	Configuration* config = malloc(sizeof(Configuration));
	t_config* temporalConfig;
	if (config_file != NULL) {
		//si es distinto a null quiere decir que recibimos el archivo de configuracion por parametro
		temporalConfig = config_create(config_file);
		if (temporalConfig == NULL) {
			error_show("No se pudo leer el archivo de configuracion %s",
					config_file);
			exit(1);
		}
	} else {
		temporalConfig = config_create(DEFAULT_CONFIG_PATH);
		if (temporalConfig == NULL) {
			//para debuggear desde eclipse
			temporalConfig = config_create(ECLIPSE_CONFIG_PATH);
			if (temporalConfig == NULL) {
				error_show("No se encontró el archivo de configuracion %s",
				ECLIPSE_CONFIG_PATH);
				exit(1);
			}
		}
	}

	config_set_int_valid_value(&config->puerto, temporalConfig, PUERTO);
	config_set_int_valid_value(&config->marcos, temporalConfig, MARCOS);
	config_set_int_valid_value(&config->marco_size, temporalConfig, MARCO_SIZE);
	config_set_int_valid_value(&config->entradas_cache, temporalConfig, ENTRADAS_CACHE);
	config_set_int_valid_value(&config->cache_x_proceso, temporalConfig, CACHE_X_PROC);
	config_set_string_valid_value(&config->reemplazo_cache, temporalConfig, REEMPLAZO_CACHE);
	config_set_int_valid_value(&config->retardo_memoria, temporalConfig, RETARDO_MEMORIA);

	//configuracion de log
	config_set_string_valid_value(&config->log_level, temporalConfig, LOG_LEVEL);
	config_set_string_valid_value(&config->log_file, temporalConfig, LOG_FILE);
	config_set_string_valid_value(&config->log_program_name, temporalConfig, LOG_PROGRAM_NAME);
	config_set_bool_valid_value(&config->log_print_console, temporalConfig, LOG_PRINT_CONSOLE);

	config_destroy(temporalConfig);

	return config;
}

Configuration* getConfiguration() {
	return config;
}
