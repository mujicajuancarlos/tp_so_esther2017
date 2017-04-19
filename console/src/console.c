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
#include "handler-user.h"

int main(int argc, char *argv[]) {

	console_struct args;
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso consola");
	initializeStruct(&args, config);

	logInfo("Imprimo bienvenida al programa");
	printWelcome();

	logInfo("Inicia el lector de comandos para el usuario");
	handleUserRequest(&args);

	return EXIT_SUCCESS;
}

void initializeStruct(console_struct* args, Configuration* config){
	args->listaProgramas = list_create();
	args->config = config;
}
