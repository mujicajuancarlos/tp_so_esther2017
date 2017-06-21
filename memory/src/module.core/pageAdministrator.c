/*
 * pageAdministrator.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#include "pageAdministrator.h"
#include <unistd.h>

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
				myEntry->procPage = -1;
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

memory_page *getGlobalMemoryPage(memory_struct* memoryStruct, int processId, int processPage) {
	int inCache = getFromCache (memoryStruct, processId, processPage);
	if (inCache != -1) {
		addEntryToCache (memoryStruct, processId, processPage, inCache);
		return list_get (memoryStruct->referenceTable, inCache);
	}

	t_list* thisProcessPages;
	thisProcessPages = list_create();
	bool onlyThisProcess(void* element) {
		memory_page* page = element;
		return (page->pid == processId);
	}

	thisProcessPages = list_filter(memoryStruct->referenceTable,
			onlyThisProcess);

	bool onlyThisPage(void* element) {
		memory_page* page = element;
		return (page->procPage == processPage);
	}

	memory_page* page = list_find (thisProcessPages, onlyThisPage);
	addEntryToCache (memoryStruct, page->pid, page->procPage, page->globPage);
	return page;
}

int processWrite(memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage(memoryStruct, dataInfo->pid,
			dataInfo->pageNumber);
	char *memAddress = globalPage->startAddress + dataInfo->offset;

	if ((dataInfo->offset + dataInfo->size) > memoryStruct->config->marco_size) {
		// segmentation fault
		return (-1);
	} else
		memcpy(memAddress, dataInfo->buffer, dataInfo->size);

	return 0;
}

int processRead(memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage(memoryStruct, dataInfo->pid,
			dataInfo->pageNumber);
	char *memAddress = globalPage->startAddress + dataInfo->offset;

	if ((dataInfo->offset + dataInfo->size) > memoryStruct->config->marco_size) {
		// segmentation fault
		return (-1);
	}
	else
		memcpy(dataInfo->buffer, memAddress, dataInfo->size);

	return 0;
}

int assignNewPages(memory_struct* memoryStruct, int processId, int pages) {
	/* asigno paginas nuevas a un proceso si me pide el kernel*/
	t_list* myProcessPages;
	myProcessPages = list_create();

	bool onlyThisProcess(void* element) {
		memory_page* page = element;
		return (page->pid == processId);
	}

	myProcessPages = list_filter(memoryStruct->referenceTable, onlyThisProcess);

	int maxPage = list_size(myProcessPages);
	int i;

	t_list *freePages;
	freePages = list_create();

	bool onlyFreePages(void* element) {
		memory_page* page = element;
		return (page->isFree);
	}

	freePages = list_filter(memoryStruct->referenceTable, onlyFreePages);

	if (list_size(freePages) >= pages) {
		for (i = 0; i < pages; i++) {
			memory_page *newPage;
			newPage = list_get(freePages, i);
			newPage->pid = processId;
			newPage->procPage = maxPage++;
			newPage->isFree = false;
			logTrace ("Se crea pagina nro %i para proceso nro %i. Pagina global: %i\n",
					newPage->procPage, newPage->pid, newPage->globPage);
		}
		return 0;
	} else {
		logError ("No hay tantas paginas disponibles");
		return (-1);
	}
}

void freePage(memory_struct* memoryStruct, int processId, int procPage) {
	memory_page* page = getGlobalMemoryPage (memoryStruct, processId, procPage);
	page->pid = 0;
	page->isFree = true;
	page->procPage = 0;
	removeEntryFromCache (memoryStruct, page->pid, page->procPage, page->globPage);
	logTrace ("Pagina %i de proceso %i ha sido liberada", procPage, processId);
}

void terminateProcess(memory_struct *memoryStruct, int processId) {
	t_list* thisProcessPages;
	thisProcessPages = list_create();

	bool onlyThisProcess(void* element) {
		memory_page* page = element;
		return (page->pid == processId);
	}

	thisProcessPages = list_filter(memoryStruct->referenceTable,
			onlyThisProcess);

	if (list_size(thisProcessPages) > 0) {
		int i;
		for (i = 0; i < list_size(thisProcessPages); i++) {
			memory_page* page = list_get (thisProcessPages, i);
			freePage (memoryStruct, page->pid, page->procPage);
		}
	}
	logInfo ("El proceso %i ha sido terminado", processId);
}
