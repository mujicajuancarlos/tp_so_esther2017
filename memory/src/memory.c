/*
 ============================================================================
 Name        : memory.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "memory.h"

#include "configuration.h"

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Configuración del proceso:");
	printf("\nPUERTO= %d\nMARCOS = %d\nMARCO_SIZE= %d\nENTRADAS_CACHE= %d\nCACHE_X_PROCESO= %d\nREEMPLAZO_CACHE= %s\nRETARDO_MEMORIA= %d\n\n"
			,config->puerto,config->marcos,config->marco_size,config->entradas_cache,config->cache_x_proceso,config->reemplazo_cache,config->retardo_memoria);

	puts("Creando el logger memory.log ...");
			t_log* logger = log_create(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	log_info(logger, "Inició el proceso correctamente!");

	/*Creo Server con las funciones de dc-commons*/

	puts("Esperando conexiones...");

	int accepted,bytes_recv;
	accepted = aceptarConexionCliente(crearSocketServer(config->puerto));

	/*Atendiendo a clientes*/

	puts("Atendiendo al cliente...");
	char *buffer = malloc(20);
	bytes_recv = recv(accepted, buffer, 20, 0);
	if (bytes_recv < 0){
		puts("Error al recibir ");
	}
	buffer[bytes_recv]='\0';
	printf("Me llegaron %d bytes con %s\n", bytes_recv, buffer);
	puts("Hola conexion nueva, Te escuche");
	free(buffer);
	close(accepted);


	for(;;);

	return EXIT_SUCCESS;
}
