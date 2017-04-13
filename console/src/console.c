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
#include "configuration/configuration.h"
#include "socket.h"

#define MAXDATASIZE 100

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Configuración del proceso:");
	printf("IP_KERNEL= %s\nPUERTO_KERNEL = %d\n\n",config->ip_kernel,config->puerto_kernel);

	puts("Creando el logger console.log ...");
		initLogMutex(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	logInfo("Inició el proceso correctamente!");

	puts("Creando Conexiones");
	int sckCliente = crearSocketConectarAKernel(config->ip_kernel,config->puerto_kernel);


	logInfo("Socket cliente consola creado");
	if((send(sckCliente,"Hello, world!\n",14,0)) == -1){
		puts("error en el envio");
	}

	return EXIT_SUCCESS;
}
