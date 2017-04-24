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

console_struct consoleStruct;

int main(int argc, char *argv[]) {


	Configuration *config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso consola");
	initializeStruct(&consoleStruct, config);

	logInfo("Imprimo bienvenida al programa");
	printWelcome();

	logInfo("Inicia el lector de comandos para el usuario");
	handleUserRequest(&consoleStruct);

	return EXIT_SUCCESS;
}

void initializeStruct(console_struct *consoleStruct, Configuration *config){
	consoleStruct->listaProgramas = list_create();
	consoleStruct->config = config;
	pthread_mutex_init(&(consoleStruct->stdoutMutex),NULL);
	pthread_mutex_init(&(consoleStruct->programsListMutex),NULL);
}
