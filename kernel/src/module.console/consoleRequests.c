/*
 * consoleRequests.c
 *
 *  Created on: 4/5/2017
 *      Author: utnso
 */

#include "consoleRequests.h"

/**
 * Se ejecuta cuando la consola envia el package con codigo COD_KC_RUN_PROGRAM_REQUEST
 *
 * Lista de acciones que realizo
 *  1. coloco el proceso en la lista de nuevos
 *  2. si el grado de multiprogramacion me lo permite continua sino queda frizado en este paso
 *  3. calculo la cantidad de paginas necesarias para el codigo
 *  4. solicito a la memoria que se me reserve n paginas (n es la suma del calculo hecho en el paso 3 + tamaño de stack)
 *  5. la memoria respondo con ok o no hay memoria
 *  	5.1. si no hay memoria finalizo la ejecucion del proceso e informo a la consola
 *  5.2. si es ok envio el codigo fuente a la memoria
 * 	6. quedo a la espera de q la memoria responda que finalizo el almanenamiento del codigo
 *	7. genero el pcb para el proceso (incluye los metadatos del parser)
 *	8. muevo el proceso a ready
 *	fin
 */
void startNewProcess(Process* process, Package* package) {

	process->pid = getNextPID();
	logInfo("Se asigno el pid: %d al proceso de FD: %d", process->pid, process->fileDescriptor);
	sendToNEW(process);

	logInfo("Verifico el grado de multiprogramacion para el pid %d", process->pid);
	_decrementMultiprogrammingLevel();
	logInfo("El pid: %d ingreso oficialmente al sistema por el grado de multiprogramacion", process->pid);

	if(canContinueNewProcessExecution(process)){

		reservePagesForNewProcess(process, package);

		sendSourceCodeForNewProcess(process, package);

		createPcbForNewProcess(process, package);

		moveFromNewToReady(process);
	}
}

void stopProcess(Process* process, Package* package){
	int state = getProcessStateIndex(process);
	if(state != STATE_CODE_NOTFOUND){
		basicForceQuitProcess(process,state);
	} else {
		logError("El proceso %d tiene un estado desconocido", process->pid);
	}
}

/*
 * debo continuar solo si el proceso no fue enviado a exit
 */
bool canContinueNewProcessExecution(Process* process){
	return getProcessStateIndex(process) == STATE_CODE_NEW;
}

