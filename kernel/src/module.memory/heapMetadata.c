/*
 * heapPage.c
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#include "heapMetadata.h"

heap_page* create_heap_page(int numberPage){
	heap_page* heapPage = malloc(sizeof(heap_page));
	heapPage->page = numberPage;
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
	metadata->startData = startData;
	metadata->dataSize = dataSize;
	metadata->isFree = true;
	return metadata;
}

void destroy_heap_metadata(heap_metadata* metadata){
	free(metadata);
}
