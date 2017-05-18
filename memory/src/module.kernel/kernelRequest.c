/*
 * handler-kernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "kernelRequest.h"
/**
 * esta funcion es invocada cuando se solicita generar espacio para un nuevo programa
 */
void startNewProcess(Package* package, memory_struct* memoryStruct){
	t_new_sourceCode_request* request = deserialize_new_sourceCode_request(package->stream);

	request->pid;
	request->stackSize;
	request->sourceCodeSize;
	request->sourceCode;

	int index = 0;
	uint32_t totalSize = request->sourceCodeSize + (request->stackSize * memoryStruct->pageSize);

	while (totalSize > 0) {
		if (newMemoryPage (memoryStruct, request->pid, index++) == -1)
			return; // devolver error porque no hay espacio para pagina nueva
		else
			totalSize-= memoryStruct->pageSize;
	}

	/**
	 *
	 *
	 *
	 * reservo paginas para este proceso y respondo al kernel
	 * 		- si pude reservar lo solicitado entonces vuelve a la función que la llamo
	 * 		- si no pude reservar devuelvo un error x ej: COD_MEMORY_FULL
	 *
	 *
	 *
	 */

	destroy_new_sourceCode_request(request);
}

/* devuelve la pagina global*/
memory_page *getGlobalMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {

	t_list* thisProcessPages;
	thisProcessPages = list_create ();

	bool *onlyThisProcess (memory_page *page) {
			return (page->pid == processId);
	}

	thisProcessPages = list_filter (memoryStruct->referenceTable, onlyThisProcess);

	bool *onlyThisPage (memory_page *page) {
		return (page->procPage == processPage);
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
void processWrite (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
	void *memAddress =  globalPage->startAddress + offset;

	/* aquí ya estaríamos ubicados en la dirección donde se nos manda a escribir */
}

/* se mandó a leer de esta dirección */
void processRead (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset) {
	memory_page *globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
	void *memAddress = globalPage->startAddress + offset;

	/* aquí comenzaría el dato que se pide leer*/

}

void memoryDump () {
	/* dump de memoria */
}

void assignNewPages (memory_struct* memoryStruct, int processId, int pages) {
	/* asigno paginas nuevas a un proceso si me pide el kernel*/
	t_list* myProcessPages;
	myProcessPages = list_create ();

	bool *onlyThisProcess (memory_page *page) {
		return (page->pid == processId);
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

void endProcess () {
	/* se deberían liberar las páginas que ocupaba el proceso */
}
