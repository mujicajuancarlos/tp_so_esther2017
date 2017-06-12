/*
 * heapPage.c
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#include "heapMetadata.h"

heap_page* create_heap_page(int numberPage, int pageSize){
	heap_page* heapPage = malloc(sizeof(heap_page));
	heapPage->page = numberPage;
	heapPage->pageSize = pageSize;
	heapPage->metadataList = list_create();
	return heapPage;
}

void destroy_heap_page(heap_page* heapPage){
	if(heapPage->metadataList != NULL){
		list_destroy_and_destroy_elements(heapPage->metadataList,
								(void*) destroy_heap_metadata);
		heapPage->metadataList = NULL;
	}
	free(heapPage);
}

heap_metadata* create_heap_metadata(uint32_t startData, uint32_t dataSize){
	heap_metadata* metadata = malloc(sizeof(heap_metadata));
	metadata->dataOffset = startData;
	metadata->dataSize = dataSize;
	metadata->isFree = true;
	return metadata;
}

void destroy_heap_metadata(heap_metadata* metadata){
	free(metadata);
}

/*
 * devuelvo true si tendo alguna metada con espacio libre y del tamaño indicado
 */
bool isHeapPageAvailable(int sizeAlloc, heap_page* page){
	bool condition(void* element){
		heap_metadata* metadata = element;
		return isHeapMetadataAvailable(sizeAlloc, metadata);
	}
	return list_any_satisfy(page->metadataList,condition);
}

/*
 * devuelvo true si metadata esta libre y el tamaño disponible alcanza para satisfacer sizeAlloc
 */
bool isHeapMetadataAvailable(int sizeAlloc, heap_metadata* metadata){
	return metadata->isFree && (metadata->dataSize >= sizeAlloc + sizeof_heapMetadata());
}

size_t sizeof_heapMetadata(){
	return sizeof(uint32_t) + sizeof(bool);
}
