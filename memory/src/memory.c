/*
 ============================================================================
 Name        : memory.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "memory.h"

#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuracion");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	printf("PUERTO= %d\nMARCOS = %d\nMARCO_SIZE= %d\nENTRADAS_CACHE= %d\nCACHE_X_PROCESO= %d\nREEMPLAZO_CACHE= %s\nRETARDO_MEMORIA= %d\n"
			,config->puerto,config->marcos,config->marco_size,config->entradas_cache,config->cache_x_proceso,config->reemplazo_cache,config->retardo_memoria);

	puts("Creando el logger memory.log");
			t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	return EXIT_SUCCESS;
}
