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
	addCPU(newCpu); //AGREGO E INCREMENTO EL SEMAFORO DE CPUS LIBRES
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
	removeCPU(newCpu); //REMUEVO Y DECREMENTO EL SEMAFORO DE CPUS LIBRES
	pthread_exit(EXIT_SUCCESS);
}

void handleCPURequest(CPU* cpu, Package* package) {
	int algorithm;
	switch (package->msgCode) {
	case COD_END_INSTRUCCION:
		algorithm = getAlgorithmIndex(cpu->kernelStruct->config->algoritmo);
		switch (algorithm) {
		case ROUND_ROBIN:
			cpu->process->quantum--;
			if (cpu->process->quantum > 0) {
				continueCurrentExcecution(cpu);
			} else {
				contextSwitch(cpu);
			}
			break;
		case FIFO:
			continueCurrentExcecution(cpu);
			break;
		default:
			logError(
					"El algoritmo de planificacion ingresado en el archivo de configuracion no es valido");
			exit(-1);
			break;
		}
		break;
	case COD_PROGRAM_FINISHED:
		programFinished(cpu, package);
		break;
	case COD_SIGNAL_DISCONNECTED:
		cpuDisconnected(cpu, package);
		break;
	case COD_GET_SHARED_VAR:
	case COD_SET_SHARED_VAR:
		logError("PENDIENTE");
		//TODO: pendiente
		break;
	case COD_MALLOC_MEMORY:
	case COD_FREE_MEMORY:
		logError("PENDIENTE");
				//TODO: pendiente
		break;
	case COD_OPEN_FD:
	case COD_DELETE_FD:
	case COD_CLOSE_FD:
	case COD_SEED_FD:
	case COD_WRITE_FD:
	case COD_READ_FD:
		/*
		 * todo: ver en detalle mas adelante
		 * no lo tengo definido:
		 * opcion a:
		 * hacero con dos mensajes: solicitud fs + solicitud de bloqueo
		 * opcion b:
		 * una unica solicitud -> fs y bloquear
		 *
		 * bloquear: liberar cpu y enviar proceso a bloqueado
		 *
		 */
		logError("PENDIENTE");
				//TODO: pendiente
		break;
	case COD_SEM_WAIT:
		executeWaitTo(cpu, package);
		break;
	case COD_SEM_SIGNAL:
		executeSignalTo(cpu, package);
		break;
	default:
		logError("La cpu %d envio un mensaje desconocido", cpu->fileDescriptor);
		/*
		 * liberar cpu y enviar proceso a ready
		 */
		removeCPU(cpu);
		break;
	}
}
