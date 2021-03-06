/*
 * processFinder.h
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_PROCESSFINDER_H_
#define MODULE_MODEL_PROCESSFINDER_H_

#include <stddef.h>
#include "process.h"
#include "../module.planning/processLifeCycle.h"

Process* getProcessAndStateIndexByPID(int, int* stackIndex);
int  getProcessStateIndex(Process* process);

#endif /* MODULE_MODEL_PROCESSFINDER_H_ */
