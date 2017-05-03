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

fileSystem_struct fsStruct;

int main(int argc, char *argv[]) {

	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Inicializado proceso kernel");
	initializeStruct(&fsStruct, config);

	logInfo("Creando socket server para file system");
	createKernelServerSocket(&fsStruct);

	handleKernel(&fsStruct);

	return EXIT_SUCCESS;
}

void createKernelServerSocket(fileSystem_struct* fsStruct) {

	fsStruct->socketServer = crearSocketServer(fsStruct->config->puerto);
	if (fsStruct->socketServer == -1) {
		logError("No se pudo crear el server para el kernel");
		exit(-1);
	}
	logInfo("Server Socket de file system esta escuchando");
}

void initializeStruct(fileSystem_struct* fsStruct, Configuration* config) {
	fsStruct->config = config;
}
