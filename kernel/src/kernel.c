/*
 ============================================================================
 Name        : kernel.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "kernel.h"
#include "configuration/configuration.h"
#include "socket/socket.h"
#include "cpu/cpu.h"



int main(int argc, char *argv[]) {

	kernel_struct args;

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	args.config = config;

	initLogMutex(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	logInfo("Inicializado proceso kernel");

	logInfo("Inicializando lista de cpu");
	args.listaCPUs = list_create();

	logInfo("Inicializando sockets");
	crearSockets(&args);

	logInfo("Creando el hilo para mantener CPU's");
	pthread_t hiloCpu;
	pthread_create(&hiloCpu, NULL, (void*) handleCPUs, &args);

	for (;;);
	return EXIT_SUCCESS;
}
