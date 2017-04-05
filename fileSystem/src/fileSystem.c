/*
 ============================================================================
 Name        : fileSystem.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "fileSystem.h"
#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Cargando configuracion");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Creando el logger fileSystem.log");
	t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	return EXIT_SUCCESS;
}
