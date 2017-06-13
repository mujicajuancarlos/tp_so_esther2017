/*
 * heapPage.h
 *
 *  Created on: 11/6/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_HEAPPAGE_H_
#define MODULE_MODEL_HEAPPAGE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <commons/collections/list.h>

typedef struct {
    int page;	//numero de pagina -> pagina inicial es pag stack + stack size
    int pageSize;
    t_list* metadataList; //lista de bloques de datos
} heap_page;

typedef struct {
	uint32_t dataOffset; //offset de inicio de datos
    uint32_t dataSize;	//tamaño del bloque datos
    bool isFree; 		//libre
} heap_metadata;

heap_page* create_heap_page(int numberPage, int pageSize);
void destroy_heap_page(heap_page* heapPage);

heap_metadata* create_heap_metadata(uint32_t startData, uint32_t dataSize);
void destroy_heap_metadata(heap_metadata* metadata);

heap_metadata* getHeapMetadataFromDataOffset(heap_page* page, uint32_t dataOffset,int* index);
heap_metadata* getAvailableHeapMetadataForPage(int allocSize, heap_page* page, int* index);

bool isFreePage(heap_page* page);
bool isAvailableHeapPage(int sizeAlloc, heap_page* page);
bool isAvailableHeapMetadata(int sizeAlloc, heap_metadata* metadata);

size_t sizeof_heapMetadata();

#endif /* MODULE_MODEL_HEAPPAGE_H_ */
