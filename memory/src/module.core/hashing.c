/*
 * hashing.c
 *
 *  Created on: 25/6/2017
 *      Author: utnso
 */

#include "hashing.h"

int getHashNumber (memory_struct* memoryStruct, int processId, int procPage) {
	int educatedGuess = 2 + (512 * 3 / memoryStruct->config->marco_size);
	int startingPoint = 200 - (processId - 999) * educatedGuess;
	int hashNumber = startingPoint + procPage;

	if (hashNumber >= memoryStruct->config->marcos) {
		while (hashNumber >= memoryStruct->config->marcos)
			hashNumber -= memoryStruct->config->marcos;
	}
	else if (hashNumber < 0) {
		while (hashNumber < 0)
			hashNumber += memoryStruct->config->marcos;
	}

	return (hashNumber);
}

int hashThis (memory_struct* memoryStruct, int processId, int procPage) {
	int hashNumber = getHashNumber (memoryStruct, processId, procPage);

	memory_page *p = list_get (memoryStruct->referenceTable, hashNumber);
	if (p->isFree)
		return hashNumber;
	else {
		logInfo ("Colisión de hashing");
		return (-1); // esta pagina ya esta ocupada
	}
}

int getHashed (memory_struct* memoryStruct, int processId, int procPage) {
	int hashNumber = getHashNumber (memoryStruct, processId, procPage);

	memory_page *p = list_get (memoryStruct->referenceTable, hashNumber);
	if (p->procPage == procPage && p->pid == processId)
		return hashNumber;
	else {
		return goForSearch (memoryStruct, processId, procPage);
		logInfo ("Colisión de hashing");
	}
}

int goForSearch (memory_struct* memoryStruct, int processId, int procPage) {
	bool desiredPage (void* element) {
		memory_page *p = element;
		if (p->pid == processId && p->procPage == procPage)
			return true;
		else
			return false;
	}
	memory_page *p = list_find (memoryStruct->referenceTable, desiredPage);

	return (p->globPage);
}
