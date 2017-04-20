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

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso Memoria");

	initializeStruct(&args, config);

	/*Creo Server con las funciones de dc-commons*/
	logInfo("Esperando conexiones...");

	args.socketServer = crearSocketServer(args.config->puerto);
	if (args.socketServer == -1) {
		logError("No se pudo crear el server de memoria");
		exit(-1);
	}
	logInfo("Server Socket de memoria esta escuchando");

	while(args.socketServer !=- 1){

		int accepted = aceptarConexionCliente(args.socketServer);
		int recibido = atiendoCliente(accepted);

		if (recibido == 3)
		{
			puts("Se conecto una CPU");

			pthread_t hiloCPU;
			pthread_create(&hiloCPU,NULL,comunicacionConCPU(),hiloCPU);
		}
		else
		{
			puts("Se conecto un Kernell");
			args.socketClientKernel = recibido;
			comunicacionConKernel(args.socketClientKernel);

		}
	}

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





/*
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
		status = sendPackage(args.socketServer, package);
		if (status != -1)
			printf("\nEnvio OK\n");
		destroyPackage(package);
	}
	free(message);
}

