/*
 * hashing.h
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#ifndef MODULE_CORE_HASHING_H_
#define MODULE_CORE_HASHING_H_

#include <unistd.h>
#include <commons/collections/list.h>
#include <dc-commons/logger.h>
#include <dc-commons/protocol-memory-kernel.h>
#include "../memoryStruct.h"

int getHashNumber (memory_struct* memoryStruct, int processId, int procPage);

int hashThis (memory_struct* memoryStruct, int processId, int procPage);

int getHashed (memory_struct* memoryStruct, int processId, int procPage);

int goForSearch (memory_struct* memoryStruct, int processId, int procPage);

#endif /* MODULE_CORE_HASHING_H_ */
