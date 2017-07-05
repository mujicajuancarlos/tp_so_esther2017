/*
 * consoleResponse.c
 *
 *  Created on: 24/5/2017
 *      Author: utnso
 */
#include "consoleResponse.h"

void consoleResponseRepulseMessage(Process* newProcess) {
	newProcess->aborted = true;
	Package* package;
	package = createAndSendPackage(newProcess->fileDescriptor,
	COD_KC_CANT_RUN_PROGRAM_RESPONSE, 0, NULL);
	destroyPackage(package);
}

void runProgramIsOK_response(Process* process) {
	char* buffer = serialize_int(process->pid);
	Package* package = createAndSendPackage(process->fileDescriptor,
	COD_KC_RUN_PROGRAM_RESPONSE, sizeof(uint32_t), buffer);
	free(buffer);
	destroyPackage(package);
}

void notifyEndProcess(Process* process) {
	if (!process->aborted && process->fileDescriptor != -1) {
		Package* package;
		char* exitCode = serialize_int(process->exit_code);
		package = createAndSendPackage(process->fileDescriptor,
		COD_KC_END_PROGRAM, sizeof(uint32_t), exitCode);
		free(exitCode);
		logInfo("Se indico la finalizacion del proceso a la Consola %d",
				process->pid);
		destroyPackage(package);
	}
}

void writeOnStdout(Process* process, t_data_FD_request* dataRequest) {
	Package* package;
	package = createAndSendPackage(process->fileDescriptor, COD_KC_PRINT_STDOUT,
			dataRequest->sizeBuffer, dataRequest->buffer);
	if (package != NULL) {
		logInfo(
				"Se indico a la consola %d imprimir el mensaje %s para el proceso %d",
				process->fileDescriptor, dataRequest->buffer, process->pid);
	} else {
		logError("No se pudo comunicar a la consola el print del proceso %d",
				process->pid);
	}
	destroyPackage(package);
}

