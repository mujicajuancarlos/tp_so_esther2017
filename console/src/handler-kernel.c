/*
 * handler-kernel.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernelRequest(Program* program, Package* package) {
	uint32_t status;
	char* message;
	switch (package->msgCode) {
	case COD_KC_RUN_PROGRAM_RESPONSE:
		program->pid = deserialize_int(package->stream);
		addProgram(program);
		printProgramStatus(program, "Inició el programa");
		logInfo("Ejecucion aceptada: PROG: %s PID: %d", program->sourceCodePath,
				program->pid);
		break;
	case COD_KC_END_PROGRAM:
		status = deserialize_int(package->stream);
		logInfo("El programa %d se ha finalizado con status code %d",program->pid, status);
		program->endDate = time(NULL);
		printProgramStatus(program, "Finalizó el programa");
		removeProgram(program);
		logInfo("La solicitud del usuario fue exitosa PID: %d", program->pid);
		logInfo("La conexion con el kernel mediante el socket %d se esta cerrando", program->fd_client);
		program->exit = true;
		break;

	case COD_KC_CANT_RUN_PROGRAM_RESPONSE:
		printProgramStatus(program, "Solicitud Rechazada");
		removeProgram(program);
		logInfo("El kernel no ha podido iniciar el programa %d ", program->pid);
		program->exit = true;
		break;

	case COD_KC_PRINT_STDOUT:
		message = string_substring_until(package->stream, package->size);
		program->printSize++;
		printMessage("\nStdout pid: %d, > %s", program->pid, message);
		free(message);
		break;
	default:
		logError("El Kernel ha enviado una solicitud no valida. Cod: %s",
				package->msgCode);
		break;
	}
}
