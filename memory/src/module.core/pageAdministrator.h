/*
 * pageAdministrator.h
 *
 *  Created on: 20/5/2017
 *      Author: utnso
 */

#ifndef PAGEADMINISTRATOR_H_
#define PAGEADMINISTRATOR_H_

#include "cacheAdministrator.h"

int assignNewPages (memory_struct* memoryStruct, int processId, int pages);

memory_page *getGlobalMemoryPage (memory_struct* memoryStruct, int processId, int processPage);

int processWrite (memory_struct* memoryStruct, t_PageBytes* dataInfo);

int processRead (memory_struct* memoryStruct, t_PageBytes* dataInfo);

void freePage(memory_struct* memoryStruct, int processId, int procPage);

void terminateProcess(memory_struct *memoryStruct, int processId);

#endif /* PAGEADMINISTRATOR_H_ */
