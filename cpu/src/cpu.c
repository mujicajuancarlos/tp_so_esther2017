/*
 ============================================================================
 Name        : cpu.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <dc-commons/socket-client.h>

#include "cpu.h"
#include "handler-kernel.h"

int main(int argc, char *argv[]) {

	cpu_struct args;

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	args.config = config;
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso CPU");

	logInfo("Creando socket cliente para memoria");
	int fd_memoria = crearSocketCliente(config->ip_memoria,
			config->puerto_memoria);
	if(fd_memoria == -1){
		logError("No se pudo establecer conexion con la memoria");
		return EXIT_FAILURE;
	}else args.socketClientMemoria = fd_memoria;

	logInfo("Creando socket cliente para kernel");
	int fd_kernel = crearSocketCliente(config->ip_kernel,
			config->puerto_kernel);
	if(fd_kernel == -1) {
		logError("No se pudo establecer conexion con el kernel");
		return EXIT_FAILURE;
	}else args.socketClientKernel = fd_kernel;

	handleKernel(&args);

	return EXIT_SUCCESS;
}
