/*
 * heapPage.c
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#include "heapMetadata.h"

heap_page* create_heap_page(int numberPage, int pageSize) {
	heap_page* heapPage = malloc(sizeof(heap_page));
	heapPage->page = numberPage;
	heapPage->pageSize = pageSize;
	heapPage->metadataList = list_create();
	return heapPage;
}

void destroy_heap_page(heap_page* heapPage) {
	if (heapPage->metadataList != NULL) {
		list_destroy_and_destroy_elements(heapPage->metadataList,
				(void*) destroy_heap_metadata);
		heapPage->metadataList = NULL;
	}
	free(heapPage);
}

heap_metadata* create_heap_metadata(uint32_t startData, uint32_t dataSize) {
	heap_metadata* metadata = malloc(sizeof(heap_metadata));
	metadata->dataOffset = startData;
	metadata->dataSize = dataSize;
	metadata->isFree = true;
	return metadata;
}

void destroy_heap_metadata(heap_metadata* metadata) {
	free(metadata);
}

heap_metadata* getHeapMetadataFromDataOffset(heap_page* page,
		uint32_t dataOffset, int* index) {
	*index = -1; //todo verificar, si rompe probar con 0
	bool condition(void* element) {
		heap_metadata* metadata = element;
		*index = *index + 1;
		return metadata->dataOffset == dataOffset;
	}
	return list_find(page->metadataList, condition);
}

/*
 * devuelvo la primer metadara con espacio suficiente para almacenar sizeAlloc
 */
heap_metadata* getAvailableHeapMetadataForPage(int allocSize, heap_page* page,
		int* index) {
	*index = -1; //todo verificar, si rompe probar con 0
	bool condition(void* element) {
		heap_metadata* metadata = element;
		*index = *index + 1;
		return isAvailableHeapMetadata(allocSize, metadata);
	}
	return list_find(page->metadataList, condition);
}

/*
 * devuelvo true si tendo alguna metada con espacio libre y del tamaño indicado
 */
bool isAvailableHeapPage(int allocSize, heap_page* page) {
	bool condition(void* element) {
		heap_metadata* metadata = element;
		return isAvailableHeapMetadata(allocSize, metadata);
	}
	return list_any_satisfy(page->metadataList, condition);
}

bool isFreePage(heap_page* page) {
	bool condition(void* element) {
		heap_metadata* metadata = element;
		return metadata->isFree;
	}
	return list_all_satisfy(page->metadataList,condition);
}

/*
 * devuelvo true si metadata esta libre y el tamaño disponible alcanza para satisfacer sizeAlloc
 */
bool isAvailableHeapMetadata(int allocSize, heap_metadata* metadata) {
	return metadata->isFree
			&& (metadata->dataSize >= allocSize + sizeof_heapMetadata());
}

size_t sizeof_heapMetadata() {
	return sizeof(uint32_t) + sizeof(bool);
}
