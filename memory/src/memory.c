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

int atiendoCliente(int numero_sck);

int main(int argc, char *argv[]) {

	memory_struct args;
	int * memoriaPrincipal; // Ver el tipo y cómo retorna la función memoriaPrincipal(); ¿Va int?

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso Memoria");

	// Se reserva espacio de memoria contigua para simular la Memoria Principal del sistema

	memoriaPrincipal = malloc(config->tamanio_memoria);
	if (memoriaPrincipal == NULL){
		logError("Error al crear la Memoria Principal");
	}


	initializeStruct(&args, config);

	// Creo hilo para el manejo de las estructuras (Comunicaciones con Kernel y CPUs)
	pthread_t hiloHandleConnection;
	pthread_create(&hiloHandleConnection,NULL,(void*) handleMemory,(config,args));



	for (;;);

	return EXIT_SUCCESS;
}

void initializeStruct(memory_struct* args, Configuration* config) {
	args->config = config;
	args->socketServer = -1;
	args->socketClientKernel = -1;
	args->listaCPUs = list_create();
}

int atiendoCliente(int aceptado) {
	char *buffer = malloc(20);
	int bytes_recv = recv(aceptado, buffer, 20, 0);
	if (bytes_recv < 0) {
		puts("Error al recibir ");
	}
	buffer[bytes_recv] = '\0';
	printf("Me llegaron %d bytes con %s\n", bytes_recv, buffer);
	puts("Te escuche");
	free(buffer);
	return bytes_recv;
}


