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
		pthread_mutex_lock(&(program->console->stdoutMutex));
		puts("No se pudo iniciar el programa, kernel no acepta conexiones.");
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		printNewLine(program->console->stdoutMutex);
		pthread_exit(NULL);
	}

	//armo paquete y lo envio, verifico que no se produzca errores
	logDebug("Creando paquete de inicio de programa para enviar a kernel.");
	char* stream = readFile(program->sourceCodePath, &fsize);
	package = createAndSendPackage(program->fd_client, COD_KC_RUN_PROGRAM_REQUEST, fsize, stream);
	free(stream);
	if (package == NULL) {
		logError("No se pudo enviar datos al kernel");
		pthread_mutex_lock(&(program->console->stdoutMutex));
		printf("No se pudo iniciar el programa, kernel no acepta peticiones.");
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		printNewLine(program->console->stdoutMutex);
		pthread_exit(NULL);
	}
	destroyPackage(package);

	//El programa ya esta en el kernel, asumo que esta corriendo y espero instrucciones
	running = true;
	while (running) {
		package = createAndReceivePackage(program->fd_client);
		if (package != NULL){
			handleKernelRequest(program, package);
		}else {
			running = false;
			logError("El kernel cerro la conexion para FD: %d",
					program->fd_client);
			close(program->fd_client);
			if (program->pid == -1) { // pid -1 correnponde a un programa que nunca llego a ejecutar
				pthread_mutex_lock(&(program->console->stdoutMutex));
				printf(
						"Se cancelo la ejecucion del programa %s, kernel no esta disponible.",
						program->sourceCodePath);
				pthread_mutex_unlock(&(program->console->stdoutMutex));
				printNewLine(program->console->stdoutMutex);
			} else {
				//llego a ejecutar pero el kernel lo cancelo
				program->endDate = time(NULL);
				pthread_mutex_lock(&(program->console->stdoutMutex));
				printf("Se cancelo la ejecucion del programa:");
				pthread_mutex_unlock(&(program->console->stdoutMutex));
				printNewLine(program->console->stdoutMutex);
				removeProgram(program);
			}
		}
		destroyPackage(package);
	}
	destroyProgram(program);
	close(program->fd_client);
	pthread_exit(EXIT_SUCCESS);
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
