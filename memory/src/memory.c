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

memory_struct memoryStruct;

int main(int argc, char *argv[]) {

	Configuration* config = config_with(argc > 1 ? argv[1] : NULL);
	initLogMutex(config->log_file, config->log_program_name,
			config->log_print_console,
			log_level_from_string(config->log_level));

	logInfo("Inicializado proceso memoria");
	initializeStruct(&memoryStruct, config);

	logInfo("Creando socket server para memoria");
	createMemoryServerSocket(&memoryStruct);

	logInfo("Creando el hilo atender a los clientes");
	pthread_t hiloClientes;
	pthread_create(&hiloClientes, NULL,(void*) handleClients, &memoryStruct);

	logInfo("Inicia el lector de comandos para el usuario");
	handleUserRequest(&memoryStruct);

	return EXIT_SUCCESS;
}

void createMemoryServerSocket(memory_struct* memoryStruct) {

	memoryStruct->socketServer = crearSocketServer(
			memoryStruct->config->puerto);
	if (memoryStruct->socketServer == -1) {
		logError("No se pudo crear el socket server");
		exit(-1);
	}
	logInfo("Server Socket de memoria esta escuchando");
}

void initializeStruct(memory_struct* memoryStruct, Configuration* config) {
	memoryStruct->config = config;
	memoryStruct->socketServer = -1;
	memoryStruct->socketClientKernel = -1;
	pthread_mutex_init(&(memoryStruct->socketClientKernelMutex), NULL);
	memoryStruct->listaCPUs = list_create();
	pthread_mutex_init(&(memoryStruct->listaCPUsMutex), NULL);

	memoryStruct->memorySize = 102400; // por ahora quiero probar harcodeado
	memoryStruct->pageSize = 512; // por ahora quiero probar harcodeado
	memoryStruct->memory = (void*) malloc (memoryStruct->memorySize);
	memoryStruct->referenceTable = list_create();

	int i;
	int totalPages = memoryStruct->memorySize / memoryStruct->pageSize;
	for (i = 0; i < 200; i++) {
		memory_page *p;
		p->globPage = i;
		p->isFree = true;
		p->pid = 0;
		p->procPage = 0;
		p->startAddress = memoryStruct->memory + memoryStruct->pageSize * i;
		list_add (memoryStruct->referenceTable, p);
	}
}
