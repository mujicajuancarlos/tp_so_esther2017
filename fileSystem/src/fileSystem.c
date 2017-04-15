/*
 ============================================================================
 Name        : fileSystem.c,
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "socket/socket.h"
#include "fileSystem.h"
#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Cargando configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Configuración del proceso:");

	//cómo mostrar bloques y punto_montaje
	printf("\nTAMANIO_BLOQUES= %d\nCANTIDAD_BLOQUES = %d\nMAGIC_NUMBER= %s\nTAMANIO= %d\nBLOQUES= %d\nPUERTO= %d\nPUNTO_MONTAJE= %d\n\n",
				config->tamanio_bloques,config->cantidad_bloques,config->magic_number,config->tamanio,config->puerto,config->punto_montaje);

	puts("Creando el logger fileSystem.log ...");
	t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");
	
	puts("Esperando conexiones...");

	int accepted, bytes_recv;
		accepted = aceptarConexionCliente(crearSocketServer(config->puerto));

		/* Atendiendo a clientes del kernel*/

		puts("Atendiendo al cliente kernel...");
		char *buffer = malloc(20);
		bytes_recv = recv(accepted, buffer, 20, 0);
		if (bytes_recv < 0){
			puts("Error al recibir ");
		}
		buffer[bytes_recv]='\0';
		printf("Me llegaron %d bytes con %s\n", bytes_recv, buffer);
		puts("Hola conexion nueva, te escuche.");
		free(buffer);
		close(accepted);

		for(;;);
		return EXIT_SUCCESS;
}
