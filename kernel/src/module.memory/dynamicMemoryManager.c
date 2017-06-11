/*
 * dynamicMemoryCore.c
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#include "dynamicMemoryManager.h"

int basicMallocMemory(Process* process, int allocSize, t_puntero* pointer){
	int status = SC_ERROR_MEMORY_EXCEPTION;

	return status;
}

int basicFreeMemory(Process* process, t_puntero pointer){
	int status = SC_ERROR_MEMORY_EXCEPTION;

	return status;
}

/*
 * busco desde el proceso, si pido una nueva pagina
 * guardo en status el estado de la ejecucion
 */
heap_page* getHeapPageForProcess(int allocSize, Process* process, int* status){
	heap_page* selectedPage = NULL;
	validateMaxAllockSize(allocSize,process,status);
	if(*status!=SC_ERROR_MEMORY_ALLOC_EXCEEDED){
		selectedPage = getHeapPageIntoListFor(allocSize, process->heapPages);
		if(selectedPage == NULL){
			selectedPage = createHeapPageFor(process, status);
		}
	}
	return selectedPage;
}

heap_page* getHeapPageIntoListFor(int allocSize, t_list* heapList){
	heap_page* selectedPage = NULL;
	return selectedPage;
}

heap_page* createHeapPageFor(Process* process, int* status){
	heap_page* selectedPage = NULL;
	return selectedPage;
}

void validateMaxAllockSize(int allocSize, Process* process, int* status){
	/*
	 * harcodeado en realidad es sizeof(heap_metadata) *2
	 * pero no modificar porque actualmente tengo un dato adicional en heap_metadata que es el inicio de dato
	 */
	int metadataMinSize = (sizeof(uint32_t) + sizeof(bool)) * 2;
	int pageSize = process->kernelStruct->pageSize;
	int utilSize = pageSize - metadataMinSize;
	*status = (utilSize >= allocSize)? MALLOC_MEMORY_SUCCES: SC_ERROR_MEMORY_ALLOC_EXCEEDED;
}

/*
 * si bien el kernel reserva el espacio la cpu manipula los datos
 * la cpu conoce y basa sus direcciones en el stackfirstpage,
 * por lo cual las direcciones del hep tmb deben basarse ene esa informacion
 */
uint32_t getHeapFistPage(Process* process){
	/*
	 * esta es la pagina cero respecto al la pagina 0 del proceso
	 * process->pcb->stackFirstPage + process->kernelStruct->config->stack_size;
	 */
	//todo: probar en detalle cuando lo ejecuta la cpu
	return  process->kernelStruct->config->stack_size;//<- esta es la pagina cero respecto del stack
}
/**
 * solo para memoria dinamica
 */
t_puntero logicalAddressToPointer(dir_memoria address, Process* process) {
	t_puntero pointer = 0;
	uint32_t heapFistPage = getHeapFistPage(process);
	pointer += (heapFistPage + address.pagina) * process->kernelStruct->pageSize;
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
