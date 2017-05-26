/*
 * pageAdministrator.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#include "pageAdministrator.h"

memory_page *getGlobalMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {

	t_list* thisProcessPages;
	thisProcessPages = list_create ();
	bool onlyThisProcess (memory_page *page) {
			return (page->pid == processId);
	}

	thisProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	bool onlyThisPage (memory_page *page) {
		return (page->procPage == processPage);
	}

	return ((memory_page*) list_find (thisProcessPages, onlyThisPage));
}

int processWrite (memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, dataInfo->pid, dataInfo->pageNumber);
	char *memAddress = globalPage->startAddress + dataInfo->offset;

	if ((dataInfo->offset + dataInfo->size) > memoryStruct->pageSize) {
		// segmentation fault
		return (-1);
	}
	else
		memcpy (memAddress, dataInfo->buffer, dataInfo->size);

	return 0;
}

int processRead (memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, dataInfo->pid, dataInfo->pageNumber);
	char *memAddress = globalPage->startAddress + dataInfo->offset;

	if ((dataInfo->offset + dataInfo->size) > memoryStruct->pageSize) {
		// segmentation fault
		return (-1);
	}
	else
		memcpy (dataInfo->buffer, memAddress, dataInfo->size);

	return 0;
}

void memoryDump (memory_struct* memoryStruct) {
	/* dump de memoria, por ahora esta hecho sólo para debuggear */
	int maxP = 20;
	int i;
	for (i = 0; i < maxP; i++) {
		memory_page *p;
		p = list_get (memoryStruct->referenceTable, i);
		if (p->isFree) {
			printf ("La pagina %i esta libre\n", p->globPage);
		}
		else {
			printf ("La pagina %i esta en uso por el proceso %i\n", p->globPage, p->pid);
		}
	}
}

int assignNewPages (memory_struct* memoryStruct, int processId, int pages) {
	/* asigno paginas nuevas a un proceso si me pide el kernel*/
	t_list* myProcessPages;
	myProcessPages = list_create ();

	bool onlyThisProcess (memory_page *page) {
		return (page->pid == processId);
	}

	myProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	int maxPage = list_size (myProcessPages);
	int i;

	t_list *freePages;
	freePages = list_create();

	bool onlyFreePages (memory_page *page) {
			return (page->isFree);
	}

	freePages = list_filter (memoryStruct->referenceTable, onlyFreePages);

	if (list_size (freePages) >= pages) {
		for (i = 0; i < pages; i++) {
			memory_page *newPage;
			newPage = list_get (freePages, i);
			newPage->pid = processId;
			newPage->procPage = maxPage++;
			newPage->isFree =false;
			list_replace (memoryStruct->referenceTable, newPage->globPage, newPage);
			printf ("Se crea pagina nro %i para proceso nro %i. Pagina global: %i\n", newPage->procPage, newPage->pid, newPage->globPage);
			// responder que se agregaron las paginas OK
		}
		return 0;
	}
	else {
		printf ("ERROR: no hay tantas paginas disponibles\n");
		// error, porque no se puede cubrir el pedido de todas esas paginas
		return (-1);
	}
}

void freePage (memory_page* page) {
	page->pid = 0;
	page->isFree = true;
	page->procPage = 0;
}

void freeProcess (memory_struct *memoryStruct, int processId) {
	t_list* thisProcessPages;
	thisProcessPages = list_create ();

	bool onlyThisProcess (memory_page *page) {
			return (page->pid == processId);
	}

	thisProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	if (list_size (thisProcessPages) > 0) {
		int i;
		for (i = 0; i < list_size (thisProcessPages); i++)
			freePage ((memory_page*) list_get (thisProcessPages, i));
	}

}
