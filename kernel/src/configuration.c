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


void VerifiedFileModificationINOTIFY(Configuration* config){

  int length, i = 0;
  int fd;
  char buffer[BUF_LEN];

  fd  = inotify_init();

  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  int watch_descriptor = inotify_add_watch( fd, CONFIG_PATH,
                           IN_MODIFY );

  length = read( fd, buffer, BUF_LEN );

  if ( length < 0 ) {
    perror( "read" );
  }

  while ( i < length ) {
	struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];

    if ( event->len ) {

     if ( event->mask & IN_MODIFY ) {

    	 if ( event->mask & IN_ISDIR ) {

        	logInfo("The directory %s was modified.\n", event->name );

    	 }
    	 else {
           	logInfo( "The file %s was modified.\n", event->name );
           	//retardo para la funcion config_with pueda tomar el archivo despues de ser liberado
           	//por el inotify
        	sleep(1);
        	Configuration* configu = config_with(NULL);
        	logInfo( "Se ha modificado el quantum_sleep y su valor actualizado es:  %d ", configu->quantum_sleep);

        }
      }
    }

   i += EVENT_SIZE + event->len;
   }

  // Agrego retardo la funcion no me vuelva a tomar el cambio anterior(si es que se hizo)
  sleep(1);
  VerifiedFileModificationINOTIFY(config);

  //logInfo("* Eliminar el directorio “/ tmp” de la lista de vigilancia. * ");
  //( void ) inotify_rm_watch( fd, watch_descriptor );
  //logInfo(" * Cerrar la instancia Inotify * ");
  //( void ) close( fd );

}

