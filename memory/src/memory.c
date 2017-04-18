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
#include "pthread.h"

#include "configuration.h"

void atiendoCliente(int numero_sck);

int main(int argc, char *argv[]) {

	puts("Accediendo al archivo de configuración...");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);

	puts("Configuración del proceso:");
	printf("\nPUERTO= %d\nMARCOS = %d\nMARCO_SIZE= %d\nENTRADAS_CACHE= %d\nCACHE_X_PROCESO= %d\nREEMPLAZO_CACHE= %s\nRETARDO_MEMORIA= %d\n\n"
			,config->puerto,config->marcos,config->marco_size,config->entradas_cache,config->cache_x_proceso,config->reemplazo_cache,config->retardo_memoria);

	puts("Creando el logger memory.log ...");
	 initLogMutex(config->log_file, config->log_program_name, config->log_print_console, log_level_from_string(config->log_level));

	logInfo("Inició el proceso correctamente!");

	/*Creo Server con las funciones de dc-commons*/
	puts("Esperando conexiones...");

	int socket = crearSocketServer(config->puerto); // Si unificamos conexiones de los dos clientes

	while(socket != -1){

	int accepted = aceptarConexionCliente(socket);

	pthread_t hiloCliente;
	pthread_create(&hiloCliente, NULL, (void*) atiendoCliente, &accepted);

	}

	for(;;);

	//forma de implementar el envio de informacion entre procesos
	int status;
	uint32_t size_message = 200;
	uint32_t code = COD_SALUDO;
	int exit = 0;
	while (exit == 0) {
		printf("\nIngrese un mensaje:\n");
		char* message = malloc(sizeof(char) * size_message);
		fgets(message, size_message, stdin);
		if (strcmp(message, "") == 0 || strcmp(message, "exit") == 0)
			exit = 1;
		else {
			Package *package = createPackage(code, message, size_message);
			status = sendPackage(socket, package);
			if (status != -1)
				printf("\nEnvio OK\n");
			destroyPackage(package);
		}
		free(message);
	}


	return EXIT_SUCCESS;
}

void atiendoCliente(int aceptado){


	puts("Atendiendo al cliente...");
	char *buffer = malloc(20);
	int bytes_recv = recv(aceptado, buffer, 20, 0);
	if (bytes_recv < 0){
		puts("Error al recibir ");
	}
	buffer[bytes_recv]='\0';
	printf("Me llegaron %d bytes con %s\n", bytes_recv, buffer);
	puts("Hola conexion nueva, Te escuche");
	free(buffer);
	close(aceptado);
}
