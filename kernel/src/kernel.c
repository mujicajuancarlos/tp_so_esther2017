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

kernel_struct kernelStruct;

int main(int argc, char *argv[]) {

	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Inicializado proceso kernel");
	initializeStruct(&kernelStruct, config);

	logInfo("Creando el hilo para monitorizar cambios en el achivo de confguracion");
	pthread_t hiloInotify;
	pthread_create(&hiloInotify, NULL, (void*) configMonitor, config);

	logInfo("Creacion de sockets cliente y server");
	createSockets(&kernelStruct);

	logInfo("Imprimo bienvenida al programa");
	printWelcome("Kernel");

	logInfo("Creando el hilo para mantener Consolas's");
	pthread_t hiloConsola;
	pthread_create(&hiloConsola, NULL, (void*) handleConsolas, &kernelStruct);

	logInfo("Creando el hilo para mantener CPU's");
	pthread_t hiloCpu;
	pthread_create(&hiloCpu, NULL, (void*) handleCPUs, &kernelStruct);

	logInfo("Creando el hilo para el planificador");
	pthread_t hiloPlanificador;
	pthread_create(&hiloPlanificador, NULL, (void*) handlePlanning,
			&kernelStruct);

	logInfo("Inicia el lector de comandos para el usuario");
	handleUserRequest(&kernelStruct);

	destroyKernelStructs(&kernelStruct, config);

	return EXIT_SUCCESS;
}

void createSockets(kernel_struct* kernelStruct) {
	createConsoleServerSocket(kernelStruct);
	createCpuServerSocket(kernelStruct);
	createMemoryClientSocket(kernelStruct);
	createFileSystemClientSocket(kernelStruct);
}

void initializeStruct(kernel_struct* kernelStruct, Configuration* config) {
	kernelStruct->config = config;
	kernelStruct->socketClientFileSystem = -1;
	kernelStruct->socketClientMemoria = -1;
	kernelStruct->socketServerCPU = -1;
	kernelStruct->socketServerConsola = -1;

	initializeCurrentPidMutex();
	initializeScheduler(kernelStruct);
	initializeCpuAdministrator();
	initializeFileSystemModule();
	initializeMemoryRequestMutex();
	initializeFileSystemRequestMutex();
	initializeExcecuteContinueMutex();
}

void destroyKernelStructs(kernel_struct* kernelStruct, Configuration* config) {

	free(kernelStruct);
	free(config);

	destroyCurrentPidMutex();
	destroyScheduler();
	destroyCpuAdministrator();
	destroyFileSystemModule();
	destroyMemoryRequestMutex();
	destroyFileSystemRequestMutex();
	destroyExcecuteContinueMutex();
}

kernel_struct* getKernelStruct() {
	return &kernelStruct;
}
