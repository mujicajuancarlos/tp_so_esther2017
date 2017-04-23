/*
 * handler-program.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-program.h"

int handleNewProgram(Program* program) {

	Package* package;

	program->fd_client = crearSocketCliente(program->console->config->ip_kernel,
			program->console->config->puerto_kernel);
	if (program->fd_client == -1) {
		logError("No se pudo establecer conexion con el kernel");
		return EXIT_FAILURE;
	}

	char* message = readFile(program->sourceCodePath);
	package = createPackage(COD_INICIAR_PROGRAMA, message, strlen(message));
	sendPackage(program->fd_client, package);
	destroyPackage(package);
	while (1) {
		package = createEmptyPackage();
		if (receivePackage(program->fd_client, package) == 0)
			handleKernelRequest(program, package);
		else {
			logError("El kernel cerro la conexion para FD: %d",
					program->fd_client);
		}
		destroyPackage(package);
	}
	return EXIT_SUCCESS;
}

void handleKernelRequest(Program *program, Package* package) {
	switch (package->msgCode) {
//	case COD_ANSISOP_IMPRIMIR:
//		logInfo("Ejecutando syscall imprimir");
//		break;
//	case COD_ANSISOP_SARAZA:
//		logInfo("Ejecutando syscall saraza");
//		break;
	case COD_SALUDO:
		logInfo("El kernel %d me envio el siguiente saludo: %s", program->fd_client, package->message);
		break;
	default:
		logError("No hubo saludo del kernel");
		break;
	}
}

Program* createNewProgram(console_struct* console, char* sourceCodePath) {
	Program *program = malloc(sizeof(Program));
	program->startDate = time(NULL);
	program->endDate = NULL;
	program->fd_client = -1;
	program->pid = -1;
	program->sourceCodePath = string_duplicate(sourceCodePath);
	program->console = console;
	return program;
}

void destroyProgram(Program* program) {
	if (program != NULL) {
		free(program->sourceCodePath);
	}
	free(program);
}
