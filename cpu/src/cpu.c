/*
 ============================================================================
 Name        : cpu.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "cpu.h"
#include "configuration.h"
#include "socket.h"

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	printf("...\n"); //por ahora no tiene info que mostrar

	puts("Creando el logger cpu.log ...");
		t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	puts("Creando conexiones...");
	crearConexiones(config);

	return EXIT_SUCCESS;
}
