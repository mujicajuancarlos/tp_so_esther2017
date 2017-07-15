/*
 * pageAdministrator.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#include "pageAdministrator.h"

pthread_mutex_t finderPageMutex;

void initializeFinderPageMutex() {
	pthread_mutex_init(&finderPageMutex, NULL);
}

void destroyFinderPageMutex() {
	pthread_mutex_destroy(&finderPageMutex);
}

void finderPageMutex_lock() {
	pthread_mutex_lock(&finderPageMutex);
}

void finderPageMutex_unlock() {
	pthread_mutex_unlock(&finderPageMutex);
}

memory_page *getGlobalMemoryPage(memory_struct* memoryStruct, int processId,
		int processPage) {
	memory_page *page;
	finderPageMutex_lock();
	int inCache = getFromCache(memoryStruct, processId, processPage);
	if (inCache != -1) {
		addEntryToCache(memoryStruct, processId, processPage, inCache);
		page = list_get(memoryStruct->referenceTable, inCache);
	} else {
		page = list_get(memoryStruct->referenceTable,
				getHashed(memoryStruct, processId, processPage));
		addEntryToCache(memoryStruct, page->pid, page->procPage,
				page->globPage);
	}
	finderPageMutex_unlock();
	return page;
}

int processWrite(memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage(memoryStruct, dataInfo->pid,
			dataInfo->pageNumber);
	char *memAddress = globalPage->startAddress + dataInfo->offset;

	if ((dataInfo->offset + dataInfo->size - 1)
			>= memoryStruct->config->marco_size) {
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

	if ((dataInfo->offset + dataInfo->size - 1)
			>= memoryStruct->config->marco_size) {
		// segmentation fault
		return (-1);
	} else
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

	int newPageNbr = list_size(myProcessPages);
	int i;

	for (i = 0; i < newPageNbr; i++) {
		bool thisParticularPage(void* element) {
			memory_page* page = element;
			if (page->procPage == i)
				return true;
			else
				return false;
		}
		if (!list_any_satisfy(myProcessPages, thisParticularPage)) {
			newPageNbr = i;
			break;
		}
	}

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
			int hashNumber = hashThis(memoryStruct, processId, newPageNbr);
			if (hashNumber != -1)
				newPage = list_get(memoryStruct->referenceTable, hashNumber);
			else
				newPage = list_get(freePages, i);
			newPage->pid = processId;
			newPage->procPage = newPageNbr++;
			newPage->isFree = false;
			logTrace(
					"Se crea pagina nro %i para proceso nro %i. Pagina global: %i\n",
					newPage->procPage, newPage->pid, newPage->globPage);
		}
		return 0;
	} else {
		logError("No hay tantas paginas disponibles");
		return (-1);
	}
}

void freePage(memory_struct* memoryStruct, int processId, int procPage) {
	memory_page* page = getGlobalMemoryPage(memoryStruct, processId, procPage);
	page->pid = 0;
	page->isFree = true;
	page->procPage = 0;
	removeEntryFromCache(memoryStruct, processId, procPage, page->globPage);
	logTrace("Pagina %i de proceso %i ha sido liberada", procPage, processId);
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
			memory_page* page = list_get(thisProcessPages, i);
			freePage(memoryStruct, page->pid, page->procPage);
		}
	}
	logInfo("El proceso %i ha sido terminado", processId);
}
