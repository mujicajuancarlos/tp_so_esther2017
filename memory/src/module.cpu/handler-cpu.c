/*
 * handler-cpu.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-cpu.h"

void handleCpu(memory_struct* memoryStruct, MemoryClient* newClient) {

	/**
	 * TODO pedir confirmación de que cuando llego al handleCPU de la cpu
	 * traemos el memory_struct
	 */

	CPU* cpu = createCPU(newClient);
	addCPU(cpu);
	Package* package;
	bool running = true;
	while (running) {

		package = createAndReceivePackage(cpu->fileDescriptor);
		if (package != NULL) {
			handleCpuRequest(memoryStruct, cpu, package);
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

void handleCpuRequest(memory_struct* memoryStruct, CPU* cpu, Package* package) {
	/**
	 * TODO completar con las funcionalidades que solicita la CPU
	 */

	/**
	 * TODO pedir confirmación de que cuando llego al handleRequest de la cpu
	 * traemos el memory_struct
	 */

	switch (package->msgCode) {
	case COD_SAVE_PAGE_BYTES_REQUEST: //para guardar bytes en una pagina
		cpuSaveData (package, memoryStruct);
		break;
	case COD_GET_PAGE_BYTES_REQUEST: // para leer bytes de una pagina
		cpuReadData (package, memoryStruct);
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
