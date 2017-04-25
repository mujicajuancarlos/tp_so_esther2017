/*
 * handler-kernel.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"
#include <dc-commons/protocol-kernel-console.h>

void handleKernelRequest(Program* program, Package* package) {
	switch (package->msgCode) {
	case COD_KC_RUN_PROGRAM_RESPONSE:
		program->pid = deserialize_int(package->stream);
		addProgram(program);
		logInfo("Ejecucion aceptada: PROG: %s PID: %d", program->sourceCodePath,
				program->pid);
		break;
	case COD_KC_STOP_PROGRAM_RESPONSE:
		//SIRVE CUANDO EL USUARIO SOLICITA FINALIZAR EL PROGRAMA DESDE CONSOLA
		break;
	case COD_KC_PRINT_STDOUT:
		//un programa solicita imprimir por pantalla. semaforizar e imprimir.
		//"para simplificar me viene el string completo para hacer puts"
		break;
	case COD_KC_END_PROGRAM:
		program->endDate = time(NULL);
		pthread_mutex_lock(&(program->console->stdoutMutex));
		printf("\tFinalizo la ejecucion del programa:\n");
		printProgram(program);
		pthread_mutex_unlock(&(program->console->stdoutMutex));
		removeProgram(program);
		logInfo("Kernel indico la finalizacion del programa %d",program->pid);
		break;
	default:
		logError("Kernel envio una solicitud no valida. Cod: %s", package->msgCode);
		break;
	}
}
