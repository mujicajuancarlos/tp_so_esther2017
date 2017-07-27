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

	atexit (atExitCleanup);

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
	initializeFinderPageMutex();
	memoryStruct->memorySize = memoryStruct->config->marcos * memoryStruct->config->marco_size;
	memoryStruct->memory = (void*) malloc (memoryStruct->memorySize);
	memoryStruct->referenceTable = list_create();
	memoryStruct->cacheEntries = list_create();

	/* se cargan estructuras administrativas: los struct de pagina y las entradas de cache */
	int i;
	int totalPages = memoryStruct->memorySize / memoryStruct->config->marco_size;
	void* placeAddress = memoryStruct->memory;
	int pagesUsed = 0;

	for (i = 0; i < totalPages; i++) {
		memory_page *p = placeAddress;
		p->globPage = i;
		p->isFree = true;
		p->pid = 0;
		p->procPage = 0;
		p->startAddress = memoryStruct->memory + memoryStruct->config->marco_size * i;
		list_add (memoryStruct->referenceTable, p);
		placeAddress += sizeof (memory_page);
		if ((i + 1) != totalPages) {
			if ((placeAddress + sizeof (memory_page)) >= (memoryStruct->memory + (memoryStruct->config->marco_size * (pagesUsed + 1)))) {
				pagesUsed++;
				placeAddress = memoryStruct->memory + memoryStruct->config->marco_size * pagesUsed;
			}
		}
	}

	if ((placeAddress + sizeof (cache_entry)) > (memoryStruct->memory + (memoryStruct->config->marco_size * (pagesUsed + 1)))) {
		pagesUsed++;
		placeAddress = memoryStruct->memory + memoryStruct->config->marco_size * pagesUsed;
	}

	for (i = 0; i < memoryStruct->config->entradas_cache; i++) {
		cache_entry *c = placeAddress;
		c->globPage = -1;
		c->pid = -1;
		c->procPage = -1;
		list_add (memoryStruct->cacheEntries, c);
		placeAddress += sizeof (cache_entry);
		if ((i + 1) != memoryStruct->config->entradas_cache) {
			if ((placeAddress + sizeof (cache_entry)) >= (memoryStruct->memory + (memoryStruct->config->marco_size * (pagesUsed + 1)))) {
				pagesUsed++;
				placeAddress = memoryStruct->memory + memoryStruct->config->marco_size * pagesUsed;
			}
		}
	}

	pagesUsed++;
	for (i = 0; i < pagesUsed; i++) {
		memory_page *p = list_get (memoryStruct->referenceTable, i);
		p->isFree = false;
		p->pid = -1;
		p->procPage = i;
	}


	logTrace ("Direccion de comienzo de memoria: %p", memoryStruct->memory);

	for (i = 0; i < totalPages; i++) {
		memory_page *p = list_get (memoryStruct->referenceTable, i);
		logTrace ("Pagina de memoria: %i Proceso: %i Pagina %i Direccion: %p Direccion de pag: %p", p->globPage, p->pid, p->procPage, p, p->startAddress);
	}

	logTrace ("Cache inicializada");
	for (i = 0; i < memoryStruct->config->entradas_cache; i++) {
		cache_entry *c = list_get (memoryStruct->cacheEntries, i);
		logTrace ("Pagina de memoria: %i Proceso: %i Pagina %i Direccion: %p", c->globPage, c->pid, c->procPage, c);
	}
}

void atExitCleanup () {
	free (memoryStruct.memory);
	list_destroy(memoryStruct.listaCPUs);
	list_destroy(memoryStruct.cacheEntries);
	list_destroy(memoryStruct.referenceTable);
	free (memoryStruct.config->log_file);
	free (memoryStruct.config->log_level);
	free (memoryStruct.config->log_program_name);
	free (memoryStruct.config->reemplazo_cache);
	free (memoryStruct.config);
}
