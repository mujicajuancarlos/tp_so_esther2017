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
			pthread_create(&hiloCPU,NULL,/* Agus aca pone la funcion que tiene que hacer CPU*/,/*Y aca las variables de la funcion*/);
		}
		else
		{
			puts("Se conecto un Kernell");
			args.socketClientKernel = recibido;
			/* aca va lo que hace memoria con kernel */

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
//Comunicación con Núcleo INICIO (RECEIVE)
int continuar = 1;
while(continuar){

Package *packageRecv;
if(receivePackage(socketKernel, packageRecv) != 0){ // SOCKETKERNEL O COMO LO HAGA CHRIS
	switch(packageRecv->msgCode)
	{
	case COD_ESCRITURA_PAGE:
		//Hace lo que tiene que hacer
		break;
	case COD_ASIGN_PAGE_PROCESS:
		//Hace lo que tiene que hacer
		break;
	case COD_FINALIZAR_PROGRAMA:
		continuar = 0;
		logDebug("El Kernel informa que finalizó el programa");
		break;
	case COD_SALUDO:
		//Hace lo que tenga que hacer
		break;
	}
	destroyPackage(packageRecv);
}
}
//Comunicación con Nucleo FIN
*/
