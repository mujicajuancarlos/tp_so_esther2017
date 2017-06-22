/*
 * cacheAdministrator.h
 *
 *  Created on: 21/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_CACHEADMINISTRATOR_H_
#define MODULE_CORE_CACHEADMINISTRATOR_H_

#include <unistd.h>
#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../memoryStruct.h"

void removeEntryFromCache (memory_struct* memoryStruct, int processId, int processPage, int globPage);

void addEntryToCache (memory_struct* memoryStruct, int processId, int processPage, int globPage);

int getFromCache (memory_struct* memoryStruct, int processId, int processPage);

#endif /* MODULE_CORE_CACHEADMINISTRATOR_H_ */
