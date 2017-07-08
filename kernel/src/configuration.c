/*
 * configuration.c
 *
 *  Created on: 3/4/2017
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
		} else {
			config->configPath = string_duplicate(config_file);
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
			} else {
				config->configPath = string_duplicate(ECLIPSE_CONFIG_PATH);
			}
		} else {
			config->configPath = string_duplicate(DEFAULT_CONFIG_PATH);
		}

	}

	config_set_int_valid_value(&config->puerto_program, temporalConfig,
	PUERTO_PROG);
	config_set_int_valid_value(&config->puerto_cpu, temporalConfig, PUERTO_CPU);
	config_set_string_valid_value(&config->ip_memoria, temporalConfig,
	IP_MEMORIA);
	config_set_int_valid_value(&config->puerto_memoria, temporalConfig,
	PUERTO_MEMORIA);
	config_set_string_valid_value(&config->ip_fs, temporalConfig, IP_FS);
	config_set_int_valid_value(&config->puerto_fs, temporalConfig, PUERTO_FS);
	config_set_int_valid_value(&config->quantum, temporalConfig, QUANTUM);
	config_set_int_valid_value(&config->quantum_sleep, temporalConfig,
	QUANTUM_SLEEP);
	config_set_string_valid_value(&config->algoritmo, temporalConfig,
	ALGORITMO);
	config_set_int_valid_value(&config->grado_multiprog, temporalConfig,
	GRADO_MULTIPROG);

	//configuro los semaforos
	char** semKeys = config_get_array_value(temporalConfig, SEM_IDS);
	char** semValues = config_get_array_value(temporalConfig, SEM_INIT);
	initializeSemaphores(semKeys, semValues);

	//	config->shared_vars
	char** sharedVarKeys = config_get_array_value(temporalConfig, SHARED_VARS);
	initializeSharedVars(sharedVarKeys);

	config_set_int_valid_value(&config->stack_size, temporalConfig, STACK_SIZE);

	//configuracion de log
	config_set_string_valid_value(&config->log_level, temporalConfig,
	LOG_LEVEL);
	config_set_string_valid_value(&config->log_file, temporalConfig, LOG_FILE);
	config_set_string_valid_value(&config->log_program_name, temporalConfig,
	LOG_PROGRAM_NAME);
	config_set_bool_valid_value(&config->log_print_console, temporalConfig,
	LOG_PRINT_CONSOLE);

	config_destroy(temporalConfig);

	return config;
}

Configuration* getConfiguration() {
	return config;
}

void configMonitor(Configuration* oldConfig) {

	int fileDescriptor;
	char buffer[BUF_LEN];

	while (true) {

		fileDescriptor = inotify_init();
		if (fileDescriptor < 0) {
			logError("No se pudo crear el fd para el inotify");
		}
		int watch_descriptor = inotify_add_watch(fileDescriptor,
				oldConfig->configPath,
				IN_MODIFY);
		if (read(fileDescriptor, buffer, BUF_LEN) < 0) {
			logError("No se pudo leer los datos del fd del inotify");
		}
		sleep(1);
		logInfo("Se modifico el archivo de configuracion");
		t_config* temporalConfig = config_create(oldConfig->configPath);
		if (temporalConfig == NULL) {
			logError("No se pudo leer el archivo de configuracion %s",
					oldConfig->configPath);
		} else {
			if (config_has_property(temporalConfig, QUANTUM_SLEEP)) {
				oldConfig->quantum_sleep = config_get_int_value(temporalConfig,
				QUANTUM_SLEEP);
				logInfo(
						"Se ha modificado el quantum_sleep y su valor actualizado es:  %d ",
						oldConfig->quantum_sleep);
			}else{
				logInfo("No se encontro la propiedad %s",QUANTUM_SLEEP);
			}
			config_destroy(temporalConfig);
		}
		inotify_rm_watch(fileDescriptor, watch_descriptor);
		close(fileDescriptor);
		sleep(1);
	}
}

