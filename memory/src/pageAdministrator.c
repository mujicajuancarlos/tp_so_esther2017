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
void processWrite (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size, char data []) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
	char *memAddress =  globalPage->startAddress + offset;

	/* este en realidad seria un array de chars que recibo */
	int i;
	for (i = 0; i < size; i++)
		memAddress[i] = data[i];

	int test = (int *) memAddress;
	// cuando el ciclo termine se transcribió el dato completo
}

/* se mandó a leer de esta dirección */
void processRead (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
	void *memAddress = globalPage->startAddress + offset;

	char* data = malloc (size);
	memcpy (data, memAddress, size);

	char bytesArray [size];
	int i;
	for (i = 0; i < size;i ++) {
		bytesArray [i] = data[i];
	}

	// aca el bytesArray es lo que se estaria mandando

	/* Una vez llega a destino para poder leer el dato habria que hacer:
	tipo_esperado* elDato = (tipo_esperado*) bytesArrayRecibidos;
	*/

	free (data);
	/* este es un arreglo de char que le tengo que poder mandar al kernel como respuesta
	 * el kernel despues sabiendo el tipo hace: type *variableName = (type*) arrayRecibido; */
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
