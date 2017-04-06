/*
 ============================================================================
 Name        : console.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "console.h"
#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuracion");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	printf("IP_KERNEL= %d\nPUERTO_KERNEL = %d\n",config->ip_kernel,config->puerto_kernel);

	puts("Creando el logger console.log");
		t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	return EXIT_SUCCESS;
}
