/*
 * pageAdministrator.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#include "pageAdministrator.h"


void addEntryToCache (memory_struct* memoryStruct, int processId, int processPage, int globPage) {
	cache_entry* entry = malloc (sizeof (entry));
	entry->globPage = globPage;
	entry->procPage = processPage;
	entry->pid = processId;
	int i;

	// la busco en la lista
	for (i = 0; i < list_size (memoryStruct->cacheEntries); i++) {
		// si ya esta en la lista lo muevo a mas reciente
		if (entry == list_get (memoryStruct->cacheEntries, i)) {
			list_remove (memoryStruct->cacheEntries, i);
			list_add (memoryStruct->cacheEntries, entry);
			return;
		}
	}

	// como no la encontré en la lista, lo tengo que agregar
	// si la lista de entradas no esta llena, solo tengo que agregarlo en el lugar de más reciente
	if (list_size (memoryStruct->cacheEntries) < memoryStruct->config->entradas_cache) {
		list_add (memoryStruct->cacheEntries, entry);
		return;
	}
	// en cambio si esta lleno entonces vuelo al menos reciente y la agrego como más reciente
	else {
		list_remove (memoryStruct->cacheEntries, 0);
		list_add (memoryStruct->cacheEntries, entry);
	}
}

int getFromCache (memory_struct* memoryStruct, int processId, int processPage) {
	cache_entry* entry;
	int i;
	for (i = 0; i < list_size (memoryStruct->cacheEntries); i++) {
		entry = list_get (memoryStruct->cacheEntries, i);
		if (entry->pid == processId && entry->procPage == processPage)
			return entry->globPage;
	}
	logInfo ("CACHE MISS");
	usleep (memoryStruct->config->retardo_memoria);
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
			list_replace(memoryStruct->referenceTable, newPage->globPage,
					newPage);
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
