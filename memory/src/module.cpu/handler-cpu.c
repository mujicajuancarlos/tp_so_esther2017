/*
 * handler-cpu.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-cpu.h"

void handleCpu(CPU* cpu) {
	addCPU(cpu);
	Package* package;
	bool running = true;
	while (running) {
		package = createAndReceivePackage(cpu->fileDescriptor);
		if (package != NULL) {
			handleCpuRequest(cpu, package);
		} else {
			running = false;
			logError("CPU cerro la conexion para FD: %d", cpu->fileDescriptor);
		}
		destroyPackage(package);
	}
	removeCPU(cpu);
	destroyCPU(cpu);
	close(cpu->fileDescriptor);
}

void handleCpuRequest(CPU* cpu, Package* package) {
	switch (package->msgCode) {
	case COD_PAGE_SIZE_REQUEST: //para guardar bytes en una pagina
		sendPageSizeToCPU(cpu);
		break;
	case COD_SAVE_PAGE_BYTES_REQUEST: //para guardar bytes en una pagina
		cpuSaveData(cpu, package);
		break;
	case COD_GET_PAGE_BYTES_REQUEST: // para leer bytes de una pagina
		cpuReadData(cpu, package);
		break;
	default:
		logError("El kernel solicito una accion desconocida FD: %d Cod: %d",
				cpu->fileDescriptor, package->msgCode);
		break;
	}
}

void addCPU(CPU* cpu) {
	pthread_mutex_lock(&(cpu->memoryStruct->listaCPUsMutex));
	list_add(cpu->memoryStruct->listaCPUs, cpu);
	pthread_mutex_unlock(&(cpu->memoryStruct->listaCPUsMutex));
}

void removeCPU(CPU* cpu) {
	bool isEqual(void* element) {
		CPU* cpu = (CPU*) element;
		return cpu->fileDescriptor == cpu->fileDescriptor;
	}
	pthread_mutex_lock(&(cpu->memoryStruct->listaCPUsMutex));
	list_remove_by_condition(cpu->memoryStruct->listaCPUs, isEqual);
	pthread_mutex_unlock(&(cpu->memoryStruct->listaCPUsMutex));
}
