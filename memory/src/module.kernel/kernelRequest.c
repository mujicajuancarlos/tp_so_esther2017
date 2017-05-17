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

/* devuelve el numero de pagina global (el que figura en la tabla de referencias), para
 * un proceso y pagina de proceso especificados. devuelve -1 si no existe la pagina de
 * proceso buscada*/
int getGlobalMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {
	int globalPage;
	for (globalPage = 0; globalPage < list_size (memoryStruct->referenceTable); globalPage++) {
		table_element *element = (table_element *) list_get (memoryStruct->referenceTable, globalPage);
		if ((element->pid == processId) && (element->procPage == processPage))
			return (globalPage);
	}
	return (-1);
}

/* devuelve la dirección de memoria donde comienza una pagina */
void *getPageAddress (memory_struct* memoryStruct, int processId, int processPage) {
	int globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);

	if (globalPage == -1)
		return (NULL);
	else
		return (memoryStruct->memory + globalPage * memoryStruct->pageSize);
}

// hay espacio en memoria para pagina nueva?
bool spaceForNewPage (memory_struct* memoryStruct) {
	int last = list_size (memoryStruct->referenceTable) + 1;
	if ((last * memoryStruct->pageSize) >= (memoryStruct->memorySize))
		return false;
	else
		return true;
}

/* le agrego una pagina nueva de memoria para el proceso especificado. devuelve -1 si no habia espacio para pagina nueva, o el
 * numero de pagina global si se añadio correctamente */
int newMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {
	if (!spaceForNewPage (memoryStruct))
		return (-1);

	// lo agrego a la tabla de referencias
	table_element *newElement = malloc (sizeof (table_element));
	newElement->pid = processId;
	newElement->procPage = processPage;
	newElement->globPage = list_size (memoryStruct->referenceTable);
	list_add (memoryStruct->referenceTable, (table_element*) newElement);

	// devuelvo el indice global
	return (newElement->globPage);
}


/* se mandó a escribir de esta dirección */
void processWrite (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset) {

	void *memAddress = getPageAddress (memoryStruct, processId, processPage) + offset;

	/* aquí ya estaríamos ubicados en la dirección donde se nos manda a escribir */
}

/* se mandó a leer de esta dirección */
void processRead (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset) {
	void *memAddress = getPageAddress (memoryStruct, processId, processPage) + offset;

	/* aquí comenzaría el dato que se pide leer*/

}

void memoryDump () {
	/* dump de memoria */
}
