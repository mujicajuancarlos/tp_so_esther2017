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
#include "cpu/cpu.h"
#include "console/console.h"
#include "socket/server.h"



int main(int argc, char *argv[]) {

	kernel_struct args;
	char *msg ="Hola, soy kernel";

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	args.config = config;

	initLogMutex(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	logInfo("Inicializado proceso kernel");

	logInfo("Inicializando lista de cpu");
	args.listaCPUs = list_create();

	/* Parte de creacion de cliente kernel para conectarse con memoria */

	logInfo("Inicializando socket cliente");
	int sckCliente = crearSocketCliente(config->ip_memoria,config->puerto_memoria);

	int len = strlen(msg);
	if(send(sckCliente,msg,len,0) != -1){
		puts(" Se mando mensaje a memoria corectamente");
	};

	/* Parte de creacion de servidor kernel para aceptar conexiones de consolas y cpu */
	logInfo("Inicializando sockets servidor");
	crearSockets(&args);

	logInfo("Creando el hilo para mantener CPU's");
	pthread_t hiloCpu;
	pthread_create(&hiloCpu, NULL, (void*) handleCPUs, &args);

	logInfo("Creando el hilo para mantener Consolas's");
	pthread_t hiloConsola;
	pthread_create(&hiloConsola, NULL, (void*) handleConsolas, &args);


	for (;;);






	return EXIT_SUCCESS;
}
