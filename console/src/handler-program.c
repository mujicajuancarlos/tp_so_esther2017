/*
 * handler-program.c
 *
 *  Created on: 19/4/2017
 *      Author: utnso
 */


#include "handler-program.h"
#include <dc-commons/protocol-kernel-console.h>

int handleNewProgram(Program* program) {
	Package* package;
	long fsize;
	bool running;
	int sendStatus, receiveStatus;
	//verifico que me pueda conectar a kernel
	program->fd_client = crearSocketCliente(program->console->config->ip_kernel,
			program->console->config->puerto_kernel);
	if (program->fd_client == -1) {
		logError("No se pudo establecer conexion con el kernel");
		pthread_mutex_lock(&(program->console->stdoutMutex));
		puts("\tNo se pudo iniciar el programa, kernel no acepta conexiones.");
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		pthread_exit(NULL);
	}

	//armo paquete y lo envio, verifico que no se produzca errores
	char* stream = readFile(program->sourceCodePath, &fsize);
	logDebug("Creando paquete de inicio de programa para enviar a kernel.");
	package = createPackage(COD_KC_RUN_PROGRAM_REQUEST, fsize, stream);
	sendStatus = sendPackage(program->fd_client, package);
	free(stream);
	destroyPackage(package);
	if (sendStatus == -1) {
		logError("No se pudo enviar datos al kernel");
		pthread_mutex_lock(&(program->console->stdoutMutex));
		puts("\tNo se pudo iniciar el programa, kernel no acepta peticiones.");
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		pthread_exit(NULL);
	}

	//El programa ya esta en el kernel, asumo que esta corriendo y espero instrucciones
	running = true;
	while (running) {
		package = createEmptyPackage();
		receiveStatus = receivePackage(program->fd_client, package);
		if (receiveStatus != -1){
			handleKernelRequest(program, package);
		}else {
			running = false;
			logError("El kernel cerro la conexion para FD: %d",
					program->fd_client);
			close(program->fd_client);
			if (program->pid == -1) { // pid -1 correnponde a un programa no recibio la aprobacion de kernel
				pthread_mutex_lock(&(program->console->stdoutMutex));
				printf(
						"\tSe cancelo la ejecucion del programa %s, kernel no esta disponible.\n",
						program->sourceCodePath);
				pthread_mutex_unlock(&(program->console->stdoutMutex));
			} else {
				program->endDate = time(NULL);
				pthread_mutex_lock(&(program->console->stdoutMutex));
				printf("\tSe cancelo la ejecucion del programa:\n");
				printProgram(program);
				pthread_mutex_unlock(&(program->console->stdoutMutex));
				removeProgram(program);
			}
		}
		destroyPackage(package);
	}
	destroyProgram(program);
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
