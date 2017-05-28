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

cpu_struct cpuStruct;

int main(int argc, char *argv[]) {

	configSignalHandlers();

	puts("Cargando archivo externo de configuration");
	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	cpuStruct.config = config;
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Iniciando el proceso CPU");
	initializeStruct(&cpuStruct, config);

	logInfo("Creacion de sockets cliente");
	createSockets(&cpuStruct);

	handleKernel(&cpuStruct);

	return EXIT_SUCCESS;
}

void createSockets(cpu_struct* cpuStruct) {
	createMemoryClientSocket(cpuStruct);
	createKernelClientSocket(cpuStruct);
}

void initializeStruct(cpu_struct* cpuStruct, Configuration* config) {
	cpuStruct->config = config;
	cpuStruct->socketClientKernel = -1;
	cpuStruct->socketClientMemoria = -1;
	initExecutionMutex();
}

void configSignalHandlers() {
	if (signal(SIGUSR1, signal_handler) == SIG_ERR)
		printf("\nNo se pudo configurar el manejador para de señales\n");
}
