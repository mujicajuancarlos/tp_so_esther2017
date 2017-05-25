/*
 * pageAdministrator.c
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#include "pageAdministrator.h"

/* devuelve la pagina global*/
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


/* le agrego una pagina nueva de memoria para el proceso especificado. devuelve -1 si no habia espacio para pagina nueva, o 0 si se añadio
 * correctamente */
int newMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {
	t_list *freePages;
	freePages = list_create();

	bool onlyFreePages (memory_page *page) {
			return (page->isFree);
	}

	freePages = list_filter (memoryStruct->referenceTable, onlyFreePages);

	if (list_size (freePages) == 0)
		return (-1); // no hay paginas libres
	else {
		memory_page *newPage;
		newPage = list_get (freePages, 0);
		newPage->pid = processId;
		newPage->procPage = processPage;
		newPage->isFree =false;
		list_replace (memoryStruct->referenceTable, newPage->globPage, newPage);
		printf ("Se crea pagina nro %i para proceso nro %i\n", newPage->procPage, newPage->pid);
		return 0;
	}
}

/* se mandó a escribir a esta dirección */
void processWrite (memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, dataInfo->pid, dataInfo->pageNumber);
	char *memAddress =  globalPage->startAddress + dataInfo->offset;

	/* verificar que no se sobrepase la pagina
	 * error si la pagina a la que salta no es del mismo proceso */
	memcpy (memAddress, dataInfo->buffer, dataInfo->size);
}

/* se mandó a leer de esta dirección */
void processRead (memory_struct* memoryStruct, t_PageBytes* dataInfo) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, dataInfo->pid, dataInfo->pageNumber);
	char *memAddress = globalPage->startAddress + dataInfo->offset;

	/* verificar que no se sobrepase la pagina
	 * error si la pagina a la que salta no es del mismo proceso */
	memcpy (dataInfo->buffer, memAddress, dataInfo->size);

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

void assignNewPages (memory_struct* memoryStruct, int processId, int pages) {
	/* asigno paginas nuevas a un proceso si me pide el kernel*/
	t_list* myProcessPages;
	myProcessPages = list_create ();

	bool onlyThisProcess (memory_page *page) {
		return (page->pid == processId);
	}

	myProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	if (list_is_empty (myProcessPages)) {
		return;// significa que no existe ese pid, salta error
	}

	int maxPage = list_size (myProcessPages);
	int i;

	for (i = 0; i < pages; i++) {
		if (newMemoryPage (memoryStruct, processId, maxPage++) == -1)
			return; // devolver error porque no hay espacio para pagina nueva
	}
	// si el ciclo termina entonces las paginas se agregaron con exito
}

void freePage (memory_page* page) {
	page->pid = 0;
	page->isFree = true;
	page->procPage = 0;
}

void endProcess (memory_struct *memoryStruct, int processId) {
	/* si terminamos un proceso, las paginas que utilizaba se vuelven a poner como disponibles para utilizar */
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
