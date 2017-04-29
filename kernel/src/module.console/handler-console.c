/*
 * console.c
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#include "handler-console.h"

void handleConsolas(kernel_struct* kernelStruct) {

	int newSocket;

	while (1) {

		newSocket = aceptarConexionCliente(kernelStruct->socketServerConsola);

		if (newSocket != -1) {
			logInfo("El proceso consola se conectó para ejecutar un programa ansisop");
			pthread_t hiloPrograma;
			pthread_attr_t threadAttr;
			pthread_attr_init(&threadAttr);
			pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
			logInfo("Creando proceso ansisop con el FD %d", newSocket);
			Process* newProcess = createProcess(newSocket, kernelStruct); //no olvidar librerar memoria al finalizar el hilo
			logDebug("Creando el hilo para ejectar proceso con FD %d", newSocket);
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
			handleProgramRequest(newProcess, package);
		} else {
			running = false;
			logError("Consola cerro la conexion para FD: %d",
					newProcess->fileDescriptor);
		}
		destroyPackage(package);
	}

	destroyProcess(newProcess);
	close(newProcess->fileDescriptor);
	pthread_exit(EXIT_SUCCESS);
}

void handleProgramRequest(Process* newProcess, Package* package) {
	Package* response;
	char* stream;
	switch (package->msgCode) {
	case COD_KC_RUN_PROGRAM_REQUEST:
		stream = serialize_int(random_number(1000,9999));
		response = createPackage(COD_KC_RUN_PROGRAM_RESPONSE, sizeof(int),
				stream);
		free(stream);
		if (sendPackage(newProcess->fileDescriptor, response) == -1)
			logError("No se pudo enviar la respuesta a la consola");
		else
			logInfo("Se respondio el pid %d para el nuevo proceso");
		break;
	case COD_KC_END_PROGRAM:
		logInfo("Terminar ejecucion");
		break;
	}
}

int random_number(int min_num, int max_num);
int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}
