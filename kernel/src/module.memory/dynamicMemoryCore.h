/*
 * dynamicMemoryCore.h
 *
 *  Created on: 10/6/2017
 *      Author: utnso
 */

#ifndef MODULE_MEMORY_DYNAMICMEMORYCORE_H_
#define MODULE_MEMORY_DYNAMICMEMORYCORE_H_

#include <parser/parser.h>
#include <dc-commons/protocol-process-exitCodes.h>

#define MALLOC_MEMORY_SUCCES 0
#define FREE_MEMORY_SUCCES 0

int basicMalloc(int mallocSize, t_puntero* pointer);

#endif /* MODULE_MEMORY_DYNAMICMEMORYCORE_H_ */
