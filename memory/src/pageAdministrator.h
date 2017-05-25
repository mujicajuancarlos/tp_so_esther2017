/*
 * pageAdministrator.h
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#ifndef PAGEADMINISTRATOR_H_
#define PAGEADMINISTRATOR_H_

#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include "memoryStruct.h"

void assignNewPages (memory_struct* memoryStruct, int processId, int pages);

int newMemoryPage (memory_struct* memoryStruct, int processId, int processPage);

void processWrite (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size, char data []);

char* processRead (memory_struct* memoryStruct, int processId, int processPage, uint32_t offset, uint32_t size);

void memoryDump (memory_struct* memoryStruct); // NO DEBERÍA ESTAR ACA PORQUE ES UNA FUNCION DE LA CONSOLA DE LA MEMORIA

void endProcess (memory_struct *memoryStruct, int processId);

#endif /* PAGEADMINISTRATOR_H_ */
