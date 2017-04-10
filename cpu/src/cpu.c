/*
 ============================================================================
 Name        : cpu.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "cpu.h"
#include "configuration.h"
#include "socket.h"

int main(int argc, char *argv[]) {

	int socketClient;
	char *msg = "Hola, me escuchas?";

	puts("Accediendo al archivo de configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	printf("...\n"); //por ahora no tiene info que mostrar

	puts("Creando el logger cpu.log ...");
		t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	puts("Creando conexiones...");
	socketClient = conectarAMemory(config->ip_memoria,config->puerto_memoria);

	int len = strlen(msg);
	if(send(socketClient,msg,len,0) == -1){
		puts("Error en el envio");
	}

	return EXIT_SUCCESS;
}
