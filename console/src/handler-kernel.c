/*
 * handler-kernel.c
 *
 *  Created on: 23/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernelRequest(Program* program, Package* package) {
	bool status;
	char* message;
	switch (package->msgCode) {
	case COD_KC_RUN_PROGRAM_RESPONSE:
		program->pid = deserialize_int(package->stream);
		addProgram(program);
		printProgramStatus(program,"Inició el programa");
		logInfo("Ejecucion aceptada: PROG: %s PID: %d", program->sourceCodePath,
				program->pid);
		break;
	case COD_KC_STOP_PROGRAM_RESPONSE:
		status = deserialize_int(package->stream);
		if (status) {
			program->endDate = time(NULL);
			printProgramStatus(program,"Finalizó el programa");
			removeProgram(program);
			logInfo("La solicitud del usuario fue exitosa PID: %d", program->pid);
			destroyProgram(program);
		} else {
			printProgramStatus(program,"Solicitud rechazada sobre el programa");
		}
		break;

	case COD_KC_CANT_RUN_PROGRAM_RESPONSE:
			printProgramStatus(program,"Solicitud Rechazada");
			removeProgram(program);
			logInfo("Kernel no pudo iniciar el programa %d", program->pid);
			destroyProgram(program);
			break;

	case COD_KC_PRINT_STDOUT:
		message = string_substring_until(package->stream, package->size);
		printMessage("\nPID: %d, > %s", program->pid, message);
		free(message);
		break;
	case COD_KC_END_PROGRAM:
		program->endDate = time(NULL);
		printProgramStatus(program,"Finalizó el programa");
		removeProgram(program);
		logInfo("Kernel indico la finalizacion del programa %d", program->pid);
		destroyProgram(program);
		break;
	default:
		logError("Kernel envio una solicitud no valida. Cod: %s",
				package->msgCode);
		break;
	}
}
