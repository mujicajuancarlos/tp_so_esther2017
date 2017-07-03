/*
 * console.c
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#include "handler-console.h"

void handleConsolas(kernel_struct* kernelStruct) {

	int newSocket;

	while (true) {

		logInfo("Esperando nuevas conexiones desde la consola.");
		newSocket = aceptarConexionCliente(kernelStruct->socketServerConsola);

		if (newSocket != -1) {
			logInfo(
					"El proceso consola se conectó para ejecutar un programa ansisop");
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			logInfo("Creando proceso ansisop con el FD %d", newSocket);
			Process* newProcess = createProcess(newSocket, kernelStruct); //no olvidar librerar memoria al finalizar el hilo
			logDebug("Creando el hilo para ejectar proceso con FD %d",
					newSocket);
			pthread_create(&hiloPrograma, &threadAttr, (void*) handleNewProcess,
					newProcess);
			logDebug("Hilo ejecutando proceso con FD %d", newSocket);
		}
	}

}

void handleNewProcess(Process* newProcess) {

	Package* package;
	bool running = true;
	while (running) {

		package = createAndReceivePackage(newProcess->fileDescriptor);
		if (package != NULL) {
			handleConsoleRequestForProcess(newProcess, package);
		} else {
			running = false;
			logError("Consola cerro la conexion para FD: %d",
					newProcess->fileDescriptor);
			stopProcess(newProcess,package);
			newProcess->exit_code = SC_ERROR_END_PROCESS_BY_DISCONECT; //segun tp la desconexion guarda el status -6
		}
		destroyPackage(package);
	}
	//no se debe destruir process porque viven para siempre en estado exit
	close(newProcess->fileDescriptor);
	pthread_exit(EXIT_SUCCESS);
}

void handleConsoleRequestForProcess(Process* process, Package* package) {
	switch (package->msgCode) {
	case COD_KC_RUN_PROGRAM_REQUEST:
		startNewProcess(process, package);
		break;
	case COD_KC_STOP_PROGRAM_REQUEST:
		logInfo("La consola solicitó finalizar el proceso %d", process->pid);
		stopProcess(process, package);
		logInfo("El proceso %d fue detenido", process->pid);
		break;
	}
}
