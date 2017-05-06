/*
 * handler-kernel.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "kernelRequest.h"
#include "../memoryStruct.h"
/**
 * esta funcion es invocada cuando se solicita generar espacio para un nuevo programa
 */
void startNewProcess(Package* package){
	t_new_sourceCode_request* request = deserialize_new_sourceCode_request(package->stream);

	request->pid;
	request->stackSize;
	request->sourceCodeSize;
	request->sourceCode;




	/**
	 *
	 *
	 *
	 * reservo paginas para este proceso y respondo al kernel
	 * 		- si pude reservar lo solicitado entonces le indico al kernel cuales fueron las paginas reservadas (lista o array de paginas)
	 * 		- si no pude reservar devuelvo lista vacia o mejor un error x ej: COD_MEMORY_FULL
	 *
	 *
	 *
	 */

	destroy_new_sourceCode_request(request);
}

/* IGNORAR DE AQUÍ PARA ABAJO POR EL MOMENTO, COMPILA PERO FALTA DEBUGGEAR, Y REUBICAR */

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

/* recibe el id de proceso, y el numero de pagina del proceso, y nos devuelve la direccion
 * de memoria donde comienza esa pagina */
void *getPageAddress (memory_struct* memoryStruct, int processId, int processPage) {
	int globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);

	if (globalPage == -1)
		return (NULL);
	else
		return (memoryStruct->memory + globalPage * memoryStruct->pageSize);
}

// pido un bool para confirmar que tengo espacio en memoria (de lo que definimos como el
// tamaño de memoria) para una nueva pagina
bool spaceForNewPage (memory_struct* memoryStruct) {
	int last = list_size (memoryStruct->referenceTable) + 1;
	if ((last * memoryStruct->pageSize) >= (memoryStruct->memorySize))
		return false;
	else
		return true;
}

/* le agrego una pagina nueva de memoria para el proceso especificado, y se la agrega a la
 * tabla de referencias. */
void newMemoryPage (memory_struct* memoryStruct, int processId, int processPage) {
	if (!spaceForNewPage (memoryStruct))
		return;

	// lo agrego a la tabla de referencias
	table_element *newElement = malloc (sizeof (table_element));
	newElement->pid = processId;
	newElement->procPage = processPage;
	newElement->globPage = list_size (memoryStruct->referenceTable);
	list_add (memoryStruct->referenceTable, (table_element*) newElement);

	// agrego el primer heap meta data - esto es la unica parte que hay que revisar de esta funcion
	heap_meta_data *metaData = memoryStruct->memory + (newElement->globPage * memoryStruct->pageSize);
	metaData->size = memoryStruct->pageSize - sizeof (heap_meta_data);
	metaData->isFree = true;


	// reads first metadata within page
	// heap_meta_data *testMetaData = (heap_meta_data*) memoryStruct->memoryPage;

	// check if aux functions are correct, should return the address starting point for this page
	// void* address = getPageAddress (memoryStruct, newElement.pid, newElement.procPage);
}

// cuando se da alerta de un nuevo proceso, el primer paso sería crearle una pagina


/* le doy un tamaño y una pagina que pertenece a un proceso. me devuelve la direccion
   de memoria donde puedo escribir ese tamaño dentro de la pagina especificada, o NULL
   si no tengo ese espacio disponible */
void *pageWriteAddress (memory_struct* memoryStruct, int globalPage, int size) {
	void *memAddress = memoryStruct->memory + globalPage * memoryStruct->pageSize;
	void *endAddress = memoryStruct->memory + (globalPage + 1) * memoryStruct->pageSize;

	while ((endAddress - memAddress) > 5) {
		heap_meta_data *metaData = (heap_meta_data*) memAddress;
		if ((metaData->isFree) && (metaData->size >= size)) {
			metaData->isFree = false;
			metaData->size = size;
			memAddress += sizeof (heap_meta_data);
			return (memAddress);
		}
		else
			memAddress += sizeof (heap_meta_data) + metaData->size;
	}

	return (NULL);
}

/* busco entre las paginas de un proceso, en cual de ellas tengo el espacio como para
 * almacenar un dato del tamaño especificado. si no tengo espacio en ninguna de ellas,
 * entonces creo una pagina nueva para ese proceso */
void processWrite (memory_struct* memoryStruct, int processId, void* newData) {
	int processPage = 0;
	int globalPage = 0;
	// VER, EL PROBLEMA AHORA ES CON EL DATO QUE SE PASA...
	// NO CALCULA BIEN EL TAMAÑO, NI INFIERE EL TIPO DE DATO
	while (globalPage != -1) {
		globalPage = getGlobalMemoryPage (memoryStruct, processId, processPage);
		// si me esta dando -1 es que hace falta una pagina nueva
		if (globalPage == -1)
			newMemoryPage (memoryStruct, processId, processPage);

		void *memAddress = pageWriteAddress (memoryStruct, globalPage, sizeof (&newData));

		if (memAddress != NULL) {
			newData = memAddress;
			void *pageAddress = getPageAddress (memoryStruct, processId, processPage);

			heap_meta_data *metaData = (memAddress + sizeof (&newData));
			metaData->isFree = true;
			metaData->size = pageAddress + memoryStruct->pageSize - (memAddress + sizeof (&newData) + sizeof (heap_meta_data));
			// y hacer saltar el bucle con flag = -1
			globalPage = -1;
		}
		processPage++;
	}
}

/* dump de la memoria completa para un proceso en particular */
void processRead (memory_struct* memoryStruct, int processId) {
	int page = 0;
	int flag = getGlobalMemoryPage (memoryStruct, processId, page);

	while (flag != -1) {
		void *memAddress = getPageAddress (memoryStruct, processId, page);
		void *endAddress = memAddress + memoryStruct->pageSize;

		printf ("Dumpeo de memoria para proceso %i\n", processId);

		while ((endAddress - memAddress) > 5) {
			heap_meta_data *metaData = (heap_meta_data*) memAddress;
			if (metaData->isFree) {
				printf ("%i bytes ocupados por metadata\n", sizeof (heap_meta_data));
				printf ("%i bytes libres\n", metaData->size);
				memAddress += sizeof (heap_meta_data) + metaData->size;
			}
			else {
				printf ("%i bytes ocupados por metadata\n", sizeof (heap_meta_data));
				printf ("%i bytes ocupados por datos de proceso\n", metaData->size);
				memAddress += sizeof (heap_meta_data) + metaData->size;
			}
		}
		page++;
		flag = getGlobalMemoryPage (memoryStruct, processId, page);
	}
}

void newProcess (memory_struct* memoryStruct, int processId) {
	newMemoryPage (memoryStruct, processId, 0);

	double *str = malloc (sizeof (double));
	(*str) = 3.55;
	processWrite (memoryStruct, processId, str);

	int *ptr = malloc (sizeof (int));
	(*ptr) = 4;
	processWrite (memoryStruct, processId, ptr);

	processRead (memoryStruct, processId);

}

