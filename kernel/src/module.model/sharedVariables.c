/*
 * sharedVariables.c
 *
 *  Created on: 7/6/2017
 *      Author: utnso
 */

#include "sharedVariables.h"

t_dictionary* sharedVars;

pthread_mutex_t sharedVarsMutex;

void initializeSharedVars(char** varsKeys) {

	pthread_mutex_init(&sharedVarsMutex, NULL);

	sharedVars = dictionary_create();
	int index = 0;
	while (varsKeys[index] != NULL) {
		int* value = malloc(sizeof(int));
		*value = SHARED_VAR_INIT_VALUE;
		dictionary_put(sharedVars, varsKeys[index], value);
		index++;
	}
}

int getSharedVar(char* varKey, int* value) {
	int status;
	pthread_mutex_lock(&sharedVarsMutex);
	if (dictionary_has_key(sharedVars, varKey)) {
		*value = *(int*) dictionary_get(sharedVars, varKey);
		status = UPDATE_VAR_SUCCESS;
	} else {
		logError("La variable compartida: %s no esta declarada en el sistema",varKey);
		status = UPDATE_VAR_FAILURE;
	}
	pthread_mutex_unlock(&sharedVarsMutex);
	return status;
}

int setSharedVar(char* varKey, int value) {
	int status;
	pthread_mutex_lock(&sharedVarsMutex);
	if (dictionary_has_key(sharedVars, varKey)) {
		*(int*)dictionary_get(sharedVars, varKey) = value;
		status = UPDATE_VAR_SUCCESS;
	} else {
		logError("La variable compartida: %s no esta declarada en el sistema",varKey);
		status = UPDATE_VAR_FAILURE;
	}
	pthread_mutex_unlock(&sharedVarsMutex);
	return status;
}
