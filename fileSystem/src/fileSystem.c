/*
 ============================================================================
 Name        : fileSystem.c,
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "fileSystem.h"
#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Cargando configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Configuración del proceso:");
	printf("\nTAMANIO_BLOQUES= %d\nCANTIDAD_BLOQUES = %d\nMAGIC_NUMBER= %s\nTAMANIO= %d\nBLOQUES= %s\nPUERTO= %d\nPUNTO_MONTAJE= %s\n\n",config->tamanio_bloques,config->cantidad_bloques,config->magic_number,config->tamanio,config->puerto,config->punto_montaje);
	//cómo mostrar bloques y punto_montaje

	puts("Creando el logger fileSystem.log ...");
	t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	return EXIT_SUCCESS;
}
