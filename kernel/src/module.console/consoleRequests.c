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
	logInfo("El proceso pid: %d ingreso oficialmente al sistema", process->pid);

	reservePagesForNewProcess(process, package);

	sendSourceCodeForNewProcess(process, package);

	//createPcbForNewProcess(process, package);

	sendToREADY(process);

//	logInfo("Enviando codigo ansisop del proceso %d a la memoria",
//			process->pid);
//	t_new_sourceCode_request* request = create_new_sourceCode_request(
//			process->pid, process->kernelStruct->config->stack_size,
//			package->size, package->stream);
//	tmpStream = serialize_new_sourceCode_request(request);
//	tmpPackage = createAndSendPackage(
//			process->kernelStruct->socketClientMemoria, COD_SAVE_NEW_PROGRAM,
//			size_new_sourceCode_request(request), tmpStream);
//	free(tmpStream);
//	destroy_new_sourceCode_request(request);
//
//	if (tmpPackage != NULL) {
//		destroyPackage(tmpPackage);
//		logInfo(
//				"Esperando el resultado de la memoria para el proceso %d correspondiente al pedido #%d",
//				process->pid, COD_SAVE_NEW_PROGRAM);
//		tmpPackage = createAndReceivePackage(
//				process->kernelStruct->socketClientMemoria);
//		if (tmpPackage != NULL && tmpPackage->msgCode == COD_SAVE_NEW_PROGRAM_OK) {
//			logInfo(
//					"Memoria indico que el codigo ansisop para el proceso %d fue exitoso",
//					process->pid);
//			destroyPackage(tmpPackage);
//
//
//
//			logInfo("Generando la metadata para el proceso %d", process->pid);
//			t_metadata_program* metadata = metadata_desde_literal(
//					package->stream); //TODO probrar si alcanza con el stream, si no funciona hay que agregarle un \0
//
//			//TODO creacion del PCB
//			//VERIFICAR
//			PCB* newPCB = create_new_PBC(process->pid, 0, metadata);
//			//Y LUEGO QUE?
//
//		} else {
//			logError(
//					"Memoria indico que el codigo ansisop para el proceso %d no se pudo procesar",
//					process->pid);
//			destroyPackage(tmpPackage);
//			hasError = true;
//		}
//	} else
//		hasError = true;
//
//	if (hasError) {
//		logInfo("Comunicando a la consola que el proceso %d no pudo iniciar",
//				process->pid);
//		//TODO verificar el enunciado para ver si es necesario responder con la descripcion del error
//		tmpStream = serialize_int(-1);
//		tmpPackage = createAndSendPackage(process->fileDescriptor,
//		COD_KC_RUN_PROGRAM_RESPONSE, sizeof(int), tmpStream);
//	} else {
//		logInfo(
//				"Comunicando a la consola que el proceso %d se programo para ejecutar",
//				process->pid);
//		tmpStream = serialize_int(process->pid);
//		tmpPackage = createAndSendPackage(process->fileDescriptor,
//		COD_KC_RUN_PROGRAM_RESPONSE, sizeof(int), tmpStream);
//		//TODO MOVER EL PROCESO AL ESTADO READY
//		//VERIFICAR
//		//sendToREADY(process);
//	}
//
//	free(tmpPackage);
//	destroyPackage(package);
}

