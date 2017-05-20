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
	bool onlyThisProcess (memory_page page) {
			return (page.pid == processId);
	}

	thisProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	bool onlyThisPage (memory_page page) {
		return (page.procPage == processPage);
	}

	return ((memory_page*) list_find (thisProcessPages, onlyThisPage));
}


/* le agrego una pagina nueva de memoria para el proceso especificado. devuelve -1 si no habia espacio para pagina nueva, o 0 si se añadio
 * correctamente */
int newMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {
	int i;
	for (i = 0; i < list_size (memoryStruct->referenceTable); i++) {
		memory_page *newPage;
		newPage = list_get (memoryStruct->referenceTable, i);
		if (newPage->isFree) {
			newPage->pid = processId;
			newPage->procPage = processPage;
			newPage->isFree = false;
			list_replace (memoryStruct->referenceTable, i, newPage);
			return 0;
		}
	}

	// si el ciclo termino significa que no había espacio, debería volver algun error
	return (-1);
}

/* se mandó a escribir a esta dirección */
void processWrite (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
	char *memAddress =  globalPage->startAddress + offset;

	/* este en realidad seria un array de chars que recibo */
	char data [size];
	int i;
	for (i = 0; i < size; i++)
		memAddress[i] = data[i];

	// cuando el ciclo termine se transcribió el dato completo
}

/* se mandó a leer de esta dirección */
void processRead (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
	char *memAddress = globalPage->startAddress + offset;

	char data [size];
	int i;
	for (i = 0;i < size; i++)
		data[i] = memAddress[i];

	/* este es un arreglo de char que le tengo que poder mandar al kernel como respuesta
	 * el kernel despues sabiendo el tipo hace: type *variableName = (type*) arrayRecibido; */
}

void memoryDump () {
	/* dump de memoria */
}

void assignNewPages (memory_struct* memoryStruct, int processId, int pages) {
	/* asigno paginas nuevas a un proceso si me pide el kernel*/
	t_list* myProcessPages;
	myProcessPages = list_create ();

	bool onlyThisProcess (memory_page page) {
		return (page.pid == processId);
	}

	myProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	if (list_is_empty (myProcessPages)) {
		// significa que no existe ese pid
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

void endProcess (memory_struct *memoryStruct, int processId, int processPage) {
	/* si terminamos un proceso, las paginas que utilizaba se vuelven a poner como disponibles para utilizar */
	t_list* thisProcessPages;
	thisProcessPages = list_create ();

	bool onlyThisProcess (memory_page page) {
			return (page.pid == processId);
	}

	thisProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	if (list_size (thisProcessPages) > 0) {
		int i;
		for (i = 0; i < list_size (thisProcessPages); i++)
			freePage ((memory_page*) list_get (thisProcessPages, i));
	}

}
