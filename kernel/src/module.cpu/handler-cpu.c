/*
 * cpu.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include "handler-cpu.h"

void handleCPUs(kernel_struct *kernelStruct) {

	int newSocket;

	while (1) {

		logInfo("Esperando nuevas conexiones de cpu's.");
		newSocket = aceptarConexionCliente(kernelStruct->socketServerCPU);

		if (newSocket != -1) {
			logInfo("Se conecto una nueva CPU");
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			logInfo("Creando cpu con FD %d", newSocket);
			CPU* newCpu = createCPU(newSocket, kernelStruct); //no olvidar librerar memoria al finalizar el hilo
			logDebug("Creando el hilo para mantener cpu con FD %d", newSocket);
			pthread_create(&hiloPrograma, &threadAttr, (void*) handleNewCPU,
					newCpu);
			logDebug("Hilo ejecutando cpu con FD %d", newSocket);
		}
	}
}

void handleNewCPU(CPU* newCpu) {

	Package* package;
	bool running = true;
	while (running) {

		package = createAndReceivePackage(newCpu->fileDescriptor);
		if (package != NULL) {
			//TODO COMPLETAR FUNCIONALIDAD
			//handleProgramRequest(newCpu, package);
		} else {
			running = false;
			logError("Se desconecto la cpu con FD: %d",
					newCpu->fileDescriptor);
		}
		destroyPackage(package);
	}

	destroyCPU(newCpu);
	close(newCpu->fileDescriptor);
	pthread_exit(EXIT_SUCCESS);
}
