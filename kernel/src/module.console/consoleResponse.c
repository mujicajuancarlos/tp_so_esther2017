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

void runProgramIsOK_response(Process* process) {
	char* buffer = serialize_int(process->pid);
	Package* package = createAndSendPackage(process->fileDescriptor,
			COD_KC_RUN_PROGRAM_RESPONSE, sizeof(uint32_t), buffer);
	free(buffer);
	destroyPackage(package);
}

void notifyEndProcess(Process* process) {
	Package* package;
	package = createAndSendPackage(process->fileDescriptor,
	COD_KC_STOP_PROGRAM_RESPONSE, 0, NULL);
	/*package = createAndSendPackage(process->fileDescriptor,
	COD_FORCE_QUIT_PROGRAM, 0, NULL);*/
	destroyPackage(package);
	close(process->fileDescriptor);
}
