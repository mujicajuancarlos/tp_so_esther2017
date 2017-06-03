/*
 * handler-memory.c
 *
 *  Created on: 27/4/2017
 *      Author: utnso
 */

#include "handler-clients.h"

/**
 * Atiendo a los diferentes clientes (Por cada cliente nuevo)
 */
void handleClients(memory_struct* memoryStruct) {

	int newSocket;
	Package* package;

	while (true) {

		logInfo("Esperando nuevas conexiones desde el kernel o la cpu");
		newSocket = aceptarConexionCliente(memoryStruct->socketServer);

		if (newSocket != -1) {

			logInfo("Se conecto un nuevo cliente");

			package = createAndSendPackage(newSocket, COD_GET_IDENTIFICATION, 0,
			NULL);

			logInfo("Creando el nuevo cliente para FD %d", newSocket);
			MemoryClient* newClient = createMemoryClient(newSocket,
					memoryStruct);
			logDebug("Creando el hilo para atender el nuevo cliente con FD %d",
					newSocket);
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			pthread_create(&hiloPrograma, &threadAttr, (void*) handleNewClient,
					newClient);
			logDebug("Hilo ejecutando proceso con FD %d", newSocket);
		}
		destroyPackage(package);
	}
}

/**
 * Mi funcion basica es definir si el nuevo cliente es un kernel, cpu, o intruso
 */
void handleNewClient(MemoryClient* newClient) {

	Package* package = createAndReceivePackage(newClient->fileDescriptor);

	switch (package->msgCode) {
	case COD_HANDSHAKE_KERNEL:
		pthread_mutex_lock(&(newClient->memoryStruct->socketClientKernelMutex));
		if (newClient->memoryStruct->socketClientKernel == -1) {
			newClient->memoryStruct->socketClientKernel =
					newClient->fileDescriptor;
		}
		pthread_mutex_unlock(
				&(newClient->memoryStruct->socketClientKernelMutex));
		if (newClient->memoryStruct->socketClientKernel
				== newClient->fileDescriptor)
			handleKernel(newClient->memoryStruct);
		else {
			logInfo(
					"No se puede atender al cliente %d porque ya existe un kernel en el sistema",
					newClient->fileDescriptor);
			close(newClient->fileDescriptor);
		}

		break;
	case COD_HANDSHAKE_CPU:
		handleCpu(newClient->memoryStruct, newClient);
		break;
	default:
		logError("El cliente con FD %d no pudo ser identificado, respondio %d",
				newClient->fileDescriptor, package->msgCode);
		break;
	}
	destroyPackage(package);
	destroyMemoryClient(newClient);
	pthread_exit(EXIT_SUCCESS);
}
