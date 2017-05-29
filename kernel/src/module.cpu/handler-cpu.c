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
	addCPU(newCpu);
	bool running = true;
	char* tmpBuffer = serialize_int(newCpu->kernelStruct->config->stack_size);
	package = createAndSendPackage(newCpu->fileDescriptor,
			COD_SET_STACK_PAGE_SIZE, sizeof(uint32_t), tmpBuffer);
	if (package == NULL)
		running = false;
	free(tmpBuffer);
	destroyPackage(package);
	while (running) {

		package = createAndReceivePackage(newCpu->fileDescriptor);
		if (package != NULL) {
			handleCPURequest(newCpu, package);
		} else {
			running = false;
			logError("Se desconecto la cpu con FD: %d", newCpu->fileDescriptor);
		}
		destroyPackage(package);
	}

	removeCPU(newCpu); //no hace falta decrementar siempre y cuando salga de error
	close(newCpu->fileDescriptor);
	pthread_exit(EXIT_SUCCESS);
}

void handleCPURequest(CPU* cpu, Package* package) {
	/*	switch (package->msgCode) {
	 case COD_CPU_END_EXECUTION:
	 // rutina de proceso finalizado -> actualizar pcb
	 markFreeCPU(cpu);
	 break;
	 case COD_CPU_END_QUANTUM:
	 // rutina para guardar el pcb
	 //rutina  para mover el proceso ready
	 markFreeCPU(cpu);
	 break;
	 case COD_CPU_MEMORY_SYSTEM_CALL:
	 //rutina de proceso bloqueado -> actualizar pcb
	 markFreeCPU(cpu);
	 break;
	 case COD_CPU_FS_SYSTEM_CALL:
	 //rutina de proceso bloqueado -> actualizar pcb
	 markFreeCPU(cpu);
	 break;
	 default:
	 logError("La cpu %d envio un mensaje desconocido", cpu->fileDescriptor);
	 markFreeCPU(cpu);
	 //todo verificar que hacemos en este caso
	 break;
	 }*/
}
