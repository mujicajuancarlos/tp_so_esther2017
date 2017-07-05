/*
 * dynamicMemoryCore.c
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#include "dynamicMemoryManager.h"

int basicMallocMemory(Process* process, int allocSize, t_puntero* pointer) {
	int status = SC_ERROR_MEMORY_EXCEPTION;
	dir_memoria address;
	heap_page* availablePage = getAvailableHeapPageForProcess(allocSize,
			process, &status);
	if (status == MALLOC_MEMORY_SUCCES) {
		int index;
		heap_metadata* availableMetadata = getAvailableHeapMetadataForPage(
				allocSize, availablePage, &index);
		saveAlloc(allocSize, availableMetadata, index, availablePage);
		address.pagina = availablePage->page;
		address.offset = availableMetadata->dataOffset;
		*pointer = logicalAddressToPointer(address, process);

		incrementCounter(&(process->processCounters->allocateTimes_Counter),1);
		incrementCounter(&(process->processCounters->allocateSize_Counter),allocSize);
	}
	return status;
}

int basicFreeMemory(Process* process, t_puntero pointer) {
	int status = SC_ERROR_MEMORY_EXCEPTION;
	int index;
	dir_memoria address = pointerToLogicalAddress(pointer, process);
	heap_page* page = list_get(process->heapPages, address.pagina);
	if (page != NULL) {
		heap_metadata* metadata = getHeapMetadataFromDataOffset(page,
				address.offset, &index);
		if (metadata != NULL) {
			metadata->isFree = true;
		}
		executeGarbageCollectorOn(page, process, &status);
		incrementCounter(&(process->processCounters->freeTimes_Counter),1);
		incrementCounter(&(process->processCounters->freeSize_Counter),metadata->dataSize);
	}else{
		status = SC_ERROR_MEMORY_EXCEPTION;
	}
	return status;
}

void executeGarbageCollectorOn(heap_page* page, Process* process, int* status) {
	if (isFreePage(page)) {
		int memoryPageNumber = process->pcb->stackFirstPage
					+ process->pcb->stackSize + page->page;
		freePageForProcess(process, memoryPageNumber, status);
		if (*status != FREE_MEMORY_SUCCES)
			logError(
					"La memoria no pudo liberar la pagina heap # %d del proceso %d",
					page->page, process->pid);
		markAsGarbage(page);
	} else {
		resolveFragmentation(process, page);
	}
}

void resolveFragmentation(Process* process, heap_page* page) {
	heap_metadata* current = NULL;
	heap_metadata* next = NULL;
	int currentIndex = 0;
	int nextIndex = 0;
	logTrace("Iniciando algoritmo de optimizacion en la pagina heap #%d",
			page->page);
	current = list_get(page->metadataList, currentIndex);
	while (current != NULL) {
		if (current->isFree) {
			logTrace(
					"El bloque de metadatos #%d esta libre se buscara optimizar los bloques siguientes",
					currentIndex);
			nextIndex = currentIndex + 1;
			next = list_get(page->metadataList, nextIndex);
			while (next != NULL && next->isFree) {
				logTrace(
						"Se detecto el bloque de metadatos #%d tambien esta libre, se eliminara e incorporara en metadatos #",
						nextIndex, currentIndex);
				current->dataSize = current->dataSize + next->dataSize
						+ sizeof_heapMetadata();
				logTrace("Se trasfirio %d bytes al bloque #%d",
						next->dataSize + sizeof_heapMetadata(), currentIndex);
				list_remove_and_destroy_element(page->metadataList, nextIndex,
						(void*) destroy_heap_metadata);
				logTrace(
						"Se elimino el bloque #%d y se procedera a verificar el siguiente bloque",
						nextIndex);
				//no asustarse como es una lista y se elimino un elemento no hace falta incrementar nextindex
				next = list_get(page->metadataList, nextIndex);
			}
		}
		currentIndex++;
		current = list_get(page->metadataList, currentIndex);
	}
	logTrace("Finalizo el algoritmo de optimizacion en la pagina heap #%d",
			page->page);
}

void saveAlloc(int allocSize, heap_metadata* metadata, int index,
		heap_page* page) {
	metadata->dataSize = allocSize;
	metadata->isFree = false;
	createHeapMetadataFor(page, metadata, index);
}

/*
 * busco desde el proceso, si pido una nueva pagina
 * guardo en status el estado de la ejecucion
 */
heap_page* getAvailableHeapPageForProcess(int allocSize, Process* process,
		int* status) {
	heap_page* selectedPage = NULL;
	validateMaxAllockSize(allocSize, process, status);
	if (*status != SC_ERROR_MEMORY_ALLOC_EXCEEDED) {
		selectedPage = getAvailableHeapPageIntoListFor(allocSize,
				process->heapPages);
		if (selectedPage == NULL)
			selectedPage = createHeapPageFor(process, status);
	}
	return selectedPage;
}

/*
 * devuelvo un pagina con espacio desde la lista o null si no hay
 */
heap_page* getAvailableHeapPageIntoListFor(int allocSize, t_list* heapList) {
	bool condition(void* element) {
		heap_page* page = element;
		return isAvailableHeapPage(allocSize, page);
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
		createHeapMetadataFor(selectedPage, NULL, -1);
		list_add_in_index(process->heapPages, nextNumber, selectedPage);
	}
	return selectedPage;
}

/*
 * agrego una metadata en la pagina, respetando el contexto actual
 * se debe invocar a esta funcion solo si se valido que tiene espacio disponible
 * si no se valido sizeData quedara negativo y logueara el error
 * estoy preparado para generar metadata incluso en el medio de la lista
 */
void createHeapMetadataFor(heap_page* page, heap_metadata* previous,
		int previousIndex) {
	int dataOffset, sizeData, maxSize;
	heap_metadata* next = list_get(page->metadataList, previousIndex + 1);
	dataOffset =
			(previous != NULL) ? previous->dataOffset + previous->dataSize : 0;
	dataOffset += sizeof_heapMetadata(); //fijo por el tamaño de la nueva metadata
	maxSize =
			(next != NULL) ?
					next->dataOffset - sizeof_heapMetadata() : page->pageSize;
	sizeData = maxSize - dataOffset;
	if (sizeData < 0)
		logError(
				"Uso incorrecto de la funcion createHeapMetadataFor(heap_page* page), se intenta crear metadata en una pagina que no tiene mas espacio libre");
	heap_metadata* metadata = create_heap_metadata(dataOffset, sizeData);
	list_add_in_index(page->metadataList, previousIndex + 1, metadata);
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
uint32_t getHeapFistPageNumber(Process* process) {
	return process->kernelStruct->config->stack_size; //<- esta es la pagina cero del heap
}
/**
 * solo para memoria dinamica
 */
t_puntero logicalAddressToPointer(dir_memoria address, Process* process) {
	t_puntero pointer = 0;
	uint32_t heapFistPage = getHeapFistPageNumber(process);
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
	uint32_t heapFistPage = getHeapFistPageNumber(process);
	int offset = pointer % pageSize;
	if(pageNumber<heapFistPage){
		logWarning("El puntero genero una direccion logica invalida");
	}
	address.pagina = pageNumber - heapFistPage;
	address.offset = offset;
	return address;
}
