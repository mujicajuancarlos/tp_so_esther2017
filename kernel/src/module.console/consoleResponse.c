/*
 * consoleResponse.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "consoleResponse.h"

void consoleResponseRepulseMessage(Process* newProcess) {

	Package* package;
	package = createAndSendPackage(newProcess->fileDescriptor,
	COD_KC_CANT_RUN_PROGRAM_RESPONSE, 0, NULL);
	destroyPackage(package);
	removeFromNEW(newProcess);
	close(newProcess->fileDescriptor);
	destroyProcess(newProcess);
}
