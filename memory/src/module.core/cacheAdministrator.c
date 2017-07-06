/*
 * cacheAdministrator.c
 *
 *  Created on: 21/6/2017
 *      Author: utnso
 */

#include "cacheAdministrator.h"


void removeEntryFromCache (memory_struct* memoryStruct, int processId, int processPage, int globPage) {
	int i;
	for (i = 0; i < memoryStruct->config->entradas_cache; i++) {
		cache_entry *c = list_get (memoryStruct->cacheEntries, i);
		if (c->globPage == globPage && c->pid == processId && c->procPage == processPage)
			break;
	}

	// la pagina no estaba cargada en cache
	if (i == memoryStruct->config->entradas_cache)
		return;
	else {
		for (; i >= 0; i--) {
			if (i == 0) {
				cache_entry *myEntry = list_get (memoryStruct->cacheEntries, i);
				myEntry->globPage = -1;
				myEntry->pid = -1;
				myEntry->procPage = -1;
			}
			else {
				cache_entry *thisPage = list_get (memoryStruct->cacheEntries, i);
				cache_entry *previousPage = list_get (memoryStruct->cacheEntries, (i - 1));
				thisPage->globPage = previousPage->globPage;
				thisPage->pid = previousPage->pid;
				thisPage->procPage = previousPage->procPage;
			}
		}
	}
}

void addEntryToCache (memory_struct* memoryStruct, int processId, int processPage, int globPage) {
	cache_entry* entry = malloc (sizeof (entry));
	entry->globPage = globPage;
	entry->procPage = processPage;
	entry->pid = processId;
	int i;

	for (i = 0; i < list_size (memoryStruct->cacheEntries); i++) {
		cache_entry *c = list_get (memoryStruct->cacheEntries, i);
		if (c->globPage == globPage && c->pid == processId && c->procPage == processPage) {
			/* ya esta en cache, deberia mover a mas reciente */
			int j;
			for (j = i; j >= 0; j--) {
				if (j == 0) {
					cache_entry *myEntry = list_get (memoryStruct->cacheEntries, j);
					myEntry->globPage = globPage;
					myEntry->pid = processId;
					myEntry->procPage = processPage;
				}
				else {
					cache_entry *thisEntry = list_get (memoryStruct->cacheEntries, j);
					cache_entry *previousEntry = list_get (memoryStruct->cacheEntries, (j - 1));
					thisEntry->globPage = previousEntry->globPage;
					thisEntry->pid = previousEntry->pid;
					thisEntry->procPage = previousEntry->procPage;
				}
			}
			return;
		}
	}

	// no la encontre en la lista, la tendria que agregar
	int counter = 0;

	// pero si ya tengo el maximo de paginas del proceso permitido en cache, tengo que volar la menos reciente
	for (i = 0; i < memoryStruct->config->entradas_cache; i++) {
		cache_entry *c = list_get (memoryStruct->cacheEntries, i);
		if (c->pid == processId)
			counter++;
		if (counter == memoryStruct->config->cache_x_proceso)
			break;
	}

	if (i == memoryStruct->config->entradas_cache)
		i--;

	for ( ; i >= 0; i--) {
		if (i == 0) {
			cache_entry *myEntry = list_get (memoryStruct->cacheEntries, i);
			myEntry->globPage = globPage;
			myEntry->pid = processId;
			myEntry->procPage = processPage;
		}
		else {
			cache_entry *thisEntry = list_get (memoryStruct->cacheEntries, i);
			cache_entry *previousEntry = list_get (memoryStruct->cacheEntries, (i - 1));
			thisEntry->globPage = previousEntry->globPage;
			thisEntry->pid = previousEntry->pid;
			thisEntry->procPage = previousEntry->procPage;
		}
	}
}

int getFromCache (memory_struct* memoryStruct, int processId, int processPage) {
	int i;
	for (i = 0; i < list_size (memoryStruct->cacheEntries); i++) {
		cache_entry* entry;
		entry = list_get (memoryStruct->cacheEntries, i);
		if (entry->pid == processId && entry->procPage == processPage)
			return entry->globPage;
	}
	logInfo ("CACHE MISS");
	// usleep acepta microsegundos asique traduzco el retardo de la memoria especificado en milisegundos
	usleep (1000 * memoryStruct->config->retardo_memoria);
	return (-1);
}
