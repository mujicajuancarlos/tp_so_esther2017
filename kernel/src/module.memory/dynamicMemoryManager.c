/*
 * dynamicMemoryCore.c
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#include "dynamicMemoryManager.h"

int basicMallocMemory(Process* process, int allocSize, t_puntero* pointer) {
	int status = SC_ERROR_MEMORY_EXCEPTION;
	heap_page* availablePage = getHeapPageForProcess(allocSize,process, &status);
	if (status == MALLOC_MEMORY_SUCCES) {
		//todo: tengo una pagina con espacio disponible, a completar
	}
	return status;
}

int basicFreeMemory(Process* process, t_puntero pointer) {
	int status = SC_ERROR_MEMORY_EXCEPTION;

	return status;
}

/*
 * busco desde el proceso, si pido una nueva pagina
 * guardo en status el estado de la ejecucion
 */
heap_page* getHeapPageForProcess(int allocSize, Process* process, int* status) {
	heap_page* selectedPage = NULL;
	validateMaxAllockSize(allocSize, process, status);
	if (*status != SC_ERROR_MEMORY_ALLOC_EXCEEDED) {
		selectedPage = getHeapPageIntoListFor(allocSize, process->heapPages);
		if (selectedPage == NULL)
			selectedPage = createHeapPageFor(process, status);
	}
	return selectedPage;
}

/*
 * devuelvo un pagina con espacio o null si no hay
 */
heap_page* getHeapPageIntoListFor(int allocSize, t_list* heapList) {
	bool condition(void* element) {
		heap_page* page = element;
		return isHeapPageAvailable(allocSize, page);
	}
	return list_find(heapList, condition);
}

/*
 * creo y relaciono una pagina nueva y consistente al process
 */
heap_page* createHeapPageFor(Process* process, int* status) {
	heap_page* selectedPage = NULL;
	reserveNewHeapPageForProcess(process, status);
	if (*status == MALLOC_MEMORY_SUCCES) {
		int nextNumber = getNextHeapPageNumber(process->heapPages);
		selectedPage = create_heap_page(nextNumber,
				process->kernelStruct->pageSize);
		createHeapMetadataFor(selectedPage);
		list_add_in_index(process->heapPages, nextNumber, selectedPage);
	}
	return selectedPage;
}

/*
 * agrego una metadata en la pagina, respetando el contexto actual
 * se debe invocar a esta funcion solo si se valido que tiene espacio disponible
 * si no se valido sizeData quedara negativo
 */
void createHeapMetadataFor(heap_page* page) {
	int dataOffset, sizeData;
	heap_metadata* last = list_get(page->metadataList,
			list_size(page->metadataList) - 1);
	dataOffset = (last != NULL) ? last->dataOffset + last->dataSize : 0;
	dataOffset += sizeof_heapMetadata(); //fijo por el tamaño de la nueva metadata
	sizeData = page->pageSize - dataOffset;
	if (sizeData < 0)
		logError(
				"Uso incorrecto de la funcion createHeapMetadataFor(heap_page* page), se intenta crear metadata en una pagina que no tiene mas espacio libre");
	heap_metadata* metadata = create_heap_metadata(dataOffset, sizeData);
	list_add(page->metadataList, metadata);
}

/*
 * verifico que el tamaño solicitado no supere el tamaño maximo para la la estructura de memdyn elegida
 */
void validateMaxAllockSize(int allocSize, Process* process, int* status) {
	int metadataMinSize = sizeof_heapMetadata() * 2; //es por dos para la estructura inicial y para la final
	int pageSize = process->kernelStruct->pageSize;
	int utilSize = pageSize - metadataMinSize;
	*status = (utilSize >= allocSize) ?
	MALLOC_MEMORY_SUCCES :
										SC_ERROR_MEMORY_ALLOC_EXCEEDED;
}

int getNextHeapPageNumber(t_list* pageList) {
	return list_size(pageList);
}

/*
 * si bien el kernel reserva el espacio la cpu manipula los datos
 * la cpu conoce y basa sus direcciones en el stackfirstpage,
 * por lo cual las direcciones del hep tmb deben basarse ene esa informacion
 */
uint32_t getHeapFistPage(Process* process) {
	/*
	 * esta es la pagina cero respecto al la pagina 0 del proceso
	 * process->pcb->stackFirstPage + process->kernelStruct->config->stack_size;
	 */
	//todo: probar en detalle cuando lo ejecuta la cpu
	return process->kernelStruct->config->stack_size; //<- esta es la pagina cero respecto del stack
}
/**
 * solo para memoria dinamica
 */
t_puntero logicalAddressToPointer(dir_memoria address, Process* process) {
	t_puntero pointer = 0;
	uint32_t heapFistPage = getHeapFistPage(process);
	pointer += (heapFistPage + address.pagina)
			* process->kernelStruct->pageSize;
	pointer += address.offset;
	return pointer;
}

/**
 * solo para memoria dinamica
 */
dir_memoria pointerToLogicalAddress(t_puntero pointer, Process* process) {
	dir_memoria address;
	int pageSize = process->kernelStruct->pageSize;
	int pageNumber = pointer / pageSize;
	uint32_t heapFistPage = getHeapFistPage(process);
	int offset = pointer % pageSize;
	address.pagina = pageNumber - heapFistPage;
	address.offset = offset;
	return address;
}
