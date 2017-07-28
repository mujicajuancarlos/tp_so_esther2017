/*
 * handler-program.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */

#include "handler-program.h"

int handleNewProgram(Program* program) {

	Package* package;
	long fsize;
	bool running;

	//verifico que me pueda conectar a kernel
	program->fd_client = crearSocketCliente(program->console->config->ip_kernel,
			program->console->config->puerto_kernel);
	if (program->fd_client == -1) {
		logError("No se pudo establecer conexion con el kernel");
		printMessage(
				"No se pudo iniciar el programa, kernel no acepta conexiones.");
		pthread_exit(NULL);
	}

	//armo paquete y lo envio, verifico que no se produzca errores
	logDebug("Creando paquete de inicio de programa para enviar a kernel.");
	fsize = fileSize(program->sourceCodePath);
	char *stream;
	stream = readFile(program->sourceCodePath, 0, fsize);
	package = createAndSendPackage(program->fd_client,
	COD_KC_RUN_PROGRAM_REQUEST, fsize, stream);
	free(stream);
	if (package == NULL) {
		logError("No se pudo enviar datos al kernel");
		printMessage(
				"No se pudo iniciar el programa, kernel no acepta peticiones.");
		pthread_exit(NULL);
	}
	destroyPackage(package);

	//El programa ya esta en el kernel, asumo que esta corriendo y espero instrucciones
	running = true;
	while (running) {
		package = createAndReceivePackage(program->fd_client);
		if (package != NULL) {
			handleKernelRequest(program, package);
			if (program->exit) {
				close(program->fd_client);
			}
		} else {
			close(program->fd_client);
			running = false;
			logWarning(
					"Se ha cerrado la conexion con el kernel para el programa %d",
					program->pid);
			removeProgram(program);
		}
		destroyPackage(package);
	}
	destroyProgram(program);
	program = NULL;
	pthread_exit(EXIT_SUCCESS);
}

void sendEndProgramRequest(Program* program, console_struct* consoleStruct) {
	char* pidStream = serialize_int(program->pid);
	Package* package = createAndSendPackage(program->fd_client,
	COD_KC_STOP_PROGRAM_REQUEST, sizeof(int), pidStream);
	if (package != NULL) {
		logInfo("Se ha enviado la solicitud para finalizar el programa %d",
				program->pid);
	} else {
		logError(
				"No se ha podido enviar la solicitud para finalizar el programa %d",
				program->pid);
		printProgramStatus(program,
				"El kernel no acepta peticiones, no se pudo procesar el pedido para el programa");
	}
	free(pidStream);
	destroyPackage(package);
}

void addProgram(Program* program) {
	pthread_mutex_lock(&(program->console->programsListMutex));
	list_add(program->console->listaProgramas, program);
	pthread_mutex_unlock(&(program->console->programsListMutex));
}

void removeProgram(Program* program) {
	bool isEqual(void* element) {
		Program* anProgram = (Program*) element;
		return anProgram->pid == program->pid;
	}
	pthread_mutex_lock(&(program->console->programsListMutex));
	list_remove_by_condition(program->console->listaProgramas, isEqual);
	pthread_mutex_unlock(&(program->console->programsListMutex));
}
