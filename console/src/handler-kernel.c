/*
 * handler-kernel.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernelRequest(Program* program, Package* package) {
	bool status;
	switch (package->msgCode) {
	case COD_KC_RUN_PROGRAM_RESPONSE:
		program->pid = deserialize_int(package->stream);
		addProgram(program);
		pthread_mutex_lock(&(program->console->stdoutMutex));
		printf("Inicio programa %s con pid %d", program->sourceCodePath,
				program->pid);
		printProgram(program);
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		printNewLine(program->console->stdoutMutex);
		logInfo("Ejecucion aceptada: PROG: %s PID: %d", program->sourceCodePath,
				program->pid);
		break;
	case COD_KC_STOP_PROGRAM_RESPONSE:
		status = deserialize_int(package->stream);
		if (status) {
			program->endDate = time(NULL);
			pthread_mutex_lock(&(program->console->stdoutMutex));
			printf("\nHa finalizado el programa:\n");
			printProgram(program);
			pthread_mutex_unlock(&(program->console->stdoutMutex));
			printNewLine(program->console->stdoutMutex);
			removeProgram(program);
			logInfo("La solicitud del usuario fue exitosa PID: %d", program->pid);
		} else {
			pthread_mutex_lock(&(program->console->stdoutMutex));
			printf("\nTu solicitud fue rechazada:\n");
			printProgram(program);
			pthread_mutex_unlock(&(program->console->stdoutMutex));
			printNewLine(program->console->stdoutMutex);
		}
		break;
	case COD_KC_PRINT_STDOUT:
		pthread_mutex_lock(&(program->console->stdoutMutex));
		printf("\nPID: %d, > %s", program->pid, package->stream);
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		printNewLine(program->console->stdoutMutex);
		break;
	case COD_KC_END_PROGRAM:
		program->endDate = time(NULL);
		pthread_mutex_lock(&(program->console->stdoutMutex));
		printf("Finalizo la ejecucion del programa:");
		printProgram(program);
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		printNewLine(program->console->stdoutMutex);
		removeProgram(program);
		logInfo("Kernel indico la finalizacion del programa %d", program->pid);
		break;
	default:
		logError("Kernel envio una solicitud no valida. Cod: %s",
				package->msgCode);
		break;
	}
}
