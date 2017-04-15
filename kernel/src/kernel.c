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
#include "handler-console.h"
#include "handler-cpu.h"
#include "handler-memory.h"

int main(int argc, char *argv[]) {

	kernel_struct args;

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	args.config = config;
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Inicializado proceso kernel");

	handleMemoria(&args);

	logInfo("Inicializando lista de cpu");
	args.listaCPUs = list_create();

	/* Parte de creacion de servidor kernel para aceptar conexiones de consolas y cpu */
	logInfo("Inicializando sockets servidor");
	crearSockets(&args);

	logInfo("Creando el hilo para mantener CPU's");
	pthread_t hiloCpu;
	pthread_create(&hiloCpu, NULL, (void*) handleCPUs, &args);

	logInfo("Creando el hilo para mantener Consolas's");
	pthread_t hiloConsola;
	pthread_create(&hiloConsola, NULL, (void*) handleConsolas, &args);

	pthread_join(hiloCpu, NULL);
	pthread_join(hiloConsola, NULL);

	return EXIT_SUCCESS;
}
