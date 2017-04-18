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
#include "handler-kernel.h"

int main(int argc, char *argv[]) {

	fileSystem_struct args;

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	args.config = config;
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso File System");

	logInfo("Creando socket server para kernel");
	int fd_fileSystem = crearSocketServer(args.config->puerto);
	if (fd_fileSystem == -1) {
		logError("No se pudo crear el socket server");
		return EXIT_FAILURE;
	} else
		args.socketServer = fd_fileSystem;

	handleKernel(&args);

	return EXIT_SUCCESS;
}
