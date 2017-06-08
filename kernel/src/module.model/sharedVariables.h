/*
 * sharedVariables.h
 *
 *  Created on: 7/6/2017
 *      Author: utnso
 */

#ifndef MODULE_MODEL_SHAREDVARIABLES_H_
#define MODULE_MODEL_SHAREDVARIABLES_H_

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/collections/dictionary.h>
#include <dc-commons/logger.h>

#define SHARED_VAR_INIT_VALUE 0
#define UPDATE_VAR_SUCCESS 1
#define UPDATE_VAR_FAILURE -1

void initializeSharedVars(char** varsKeys);

int getSharedVar(char* varKey, int* value);

int setSharedVar(char* varKey, int value);

#endif /* MODULE_MODEL_SHAREDVARIABLES_H_ */
