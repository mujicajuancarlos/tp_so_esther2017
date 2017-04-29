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
#include "handler-user.h"
#include "handler-console.h"
#include "handler-cpu.h"
#include "handler-memory.h"
#include "handler-fileSystem.h"

kernel_struct kernelStruct;

int main(int argc, char *argv[]) {

	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Inicializado proceso kernel");
	initializeStruct(&kernelStruct, config);

	logInfo("Imprimo bienvenida al programa");
	printWelcome("Kernel");

	handleMemoria(&kernelStruct);
	handleFileSystem(&kernelStruct);

	logInfo("Inicializando lista de cpu");

	logInfo("Creando el hilo para mantener CPU's");
	pthread_t hiloCpu;
	pthread_create(&hiloCpu, NULL, (void*) handleCPUs, &kernelStruct);

	logInfo("Creando el hilo para mantener Consolas's");
	pthread_t hiloConsola;
	pthread_create(&hiloConsola, NULL, (void*) handleConsolas, &kernelStruct);

	logInfo("Inicia el lector de comandos para el usuario");
	handleUserRequest(&kernelStruct);

	return EXIT_SUCCESS;
}

void initializeStruct(kernel_struct* kernelStruct, Configuration* config){
	kernelStruct->config = config;
	inicializarArray(MAX_CPUS, kernelStruct->cpuSockets);
	inicializarArray(MAX_CONSOLAS, kernelStruct->consolaSockets);
	kernelStruct->cpuList = list_create();
	kernelStruct->processList = list_create();
	kernelStruct->socketClientFileSystem = -1;
	kernelStruct->socketClientMemoria = -1;
	kernelStruct->socketServerCPU = -1;
	kernelStruct->socketServerConsola = -1;

	//server socket para atender los pedidos de la consola
	crearServerSocketParaConsola(kernelStruct);
	//server socket para atender los pedidos del cpu
	crearServerSocketParaCpus(kernelStruct);

}
