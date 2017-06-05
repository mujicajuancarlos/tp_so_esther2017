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
#include <dc-commons/protocol-memory-kernel.h>
#include "../memoryStruct.h"

int assignNewPages (memory_struct* memoryStruct, int processId, int pages);

memory_page *getGlobalMemoryPage (memory_struct* memoryStruct, int processId, int processPage);

int processWrite (memory_struct* memoryStruct, t_PageBytes* dataInfo);

int processRead (memory_struct* memoryStruct, t_PageBytes* dataInfo);

void memoryDump (memory_struct* memoryStruct); // NO DEBERÍA ESTAR ACA PORQUE ES UNA FUNCION DE LA CONSOLA DE LA MEMORIA

void freePage (memory_page* page);

void freeProcess (memory_struct *memoryStruct, int processId);

#endif /* PAGEADMINISTRATOR_H_ */
