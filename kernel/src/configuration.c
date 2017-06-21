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

	config_set_int_valid_value(&config->puerto_program, temporalConfig, PUERTO_PROG);
	config_set_int_valid_value(&config->puerto_cpu, temporalConfig, PUERTO_CPU);
	config_set_string_valid_value(&config->ip_memoria, temporalConfig, IP_MEMORIA);
	config_set_int_valid_value(&config->puerto_memoria, temporalConfig, PUERTO_MEMORIA);
	config_set_string_valid_value(&config->ip_fs, temporalConfig, IP_FS);
	config_set_int_valid_value(&config->puerto_fs, temporalConfig, PUERTO_FS);
	config_set_int_valid_value(&config->quantum, temporalConfig, QUANTUM);
	config_set_int_valid_value(&config->quantum_sleep, temporalConfig, QUANTUM_SLEEP);
	config_set_string_valid_value(&config->algoritmo, temporalConfig, ALGORITMO);
	config_set_int_valid_value(&config->grado_multiprog, temporalConfig, GRADO_MULTIPROG);

	//configuro los semaforos
	char** semKeys = config_get_array_value(temporalConfig,SEM_IDS);
	char** semValues = config_get_array_value(temporalConfig,SEM_INIT);
	initializeSemaphores(semKeys,semValues);

	//	config->shared_vars
	char** sharedVarKeys = config_get_array_value(temporalConfig,SHARED_VARS);
	initializeSharedVars(sharedVarKeys);

	config_set_int_valid_value(&config->stack_size, temporalConfig, STACK_SIZE);

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


void VerifiedFileModification(char *config_file){

	char buffer[BUF_LEN];

		// Al inicializar inotify este nos devuelve un descriptor de archivo
		int file_descriptor = inotify_init();
		if (file_descriptor < 0) {
			perror("inotify_init");
		}

		// Creamos un monitor sobre un path indicando que eventos queremos escuchar
		int watch_descriptor = inotify_add_watch(file_descriptor, "../kernel.conf", IN_MODIFY | IN_CREATE | IN_DELETE);

		// El file descriptor creado por inotify, es el que recibe la información sobre los eventos ocurridos
		// para leer esta información el descriptor se lee como si fuera un archivo comun y corriente pero
		// la diferencia esta en que lo que leemos no es el contenido de un archivo sino la información
		// referente a los eventos ocurridos
		int length = read(file_descriptor, buffer, BUF_LEN);
		if (length < 0) {
			perror("read");
		}

		int offset = 0;

		while (offset < length) {

			struct inotify_event *event = (struct inotify_event *) &buffer[offset];

			if (event->len) {
				// Dentro de "mask" tenemos el evento que ocurrio
				if (event->mask & IN_MODIFY) {
					if (event->mask & IN_ISDIR) {
						config_with(config_file);
						printf("The file %s was modified.\n", event->name);
						}
				}
			}
			offset += sizeof (struct inotify_event) + event->len;
		}

		inotify_rm_watch(file_descriptor, watch_descriptor);
		close(file_descriptor);

}

