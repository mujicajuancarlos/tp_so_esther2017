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
		printMessage("No se pudo iniciar el programa, kernel no acepta conexiones.");
		pthread_exit(NULL);
	}

	//armo paquete y lo envio, verifico que no se produzca errores
	logDebug("Creando paquete de inicio de programa para enviar a kernel.");
	fsize = fileSize(program->sourceCodePath);
	char* stream = readFile(program->sourceCodePath, 0, fsize);
	package = createAndSendPackage(program->fd_client, COD_KC_RUN_PROGRAM_REQUEST, fsize, stream);
	free(stream);
	if (package == NULL) {
		logError("No se pudo enviar datos al kernel");
		printMessage("No se pudo iniciar el programa, kernel no acepta peticiones.");
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
				printMessage("Se cancelo la ejecucion del programa %s, kernel no esta disponible.", program->sourceCodePath);
			} else {
				//llego a ejecutar pero el kernel lo cancelo
				program->endDate = time(NULL);
				printProgramStatus(program, "Se cancelo la ejecucion del programa");
				removeProgram(program);
			}
		}
		destroyPackage(package);
	}
	destroyProgram(program);
	close(program->fd_client);
	pthread_exit(EXIT_SUCCESS);
}

void sendEndProgramRequest(Program* program, console_struct* consoleStruct){
	char* pidStream = serialize_int(program->pid);
	Package* package = createAndSendPackage(program->fd_client, COD_KC_STOP_PROGRAM_REQUEST, sizeof(int), pidStream);
	if(package != NULL){
		logInfo("Se envio la solicitud para finalizar el programa %d", program->pid);
	}else{
		logError("No se envio la solicitud para finalizar el programa %d", program->pid);
		printProgramStatus(program, "Kernel no acepta peticiones, no se pudo procesar el pedido para el programa");
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
