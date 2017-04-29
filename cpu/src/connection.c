/*
 * connection.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */


#include "connection.h"


/**
 * Creo el socket client para conectar con memoria
 */
void createMemoryClientSocket(cpu_struct* cpuStruct) {

	logInfo("Estableciendo conexion con memoria");

	cpuStruct->socketClientMemoria = crearSocketCliente(
			cpuStruct->config->ip_memoria,
			cpuStruct->config->puerto_memoria);
	if (cpuStruct->socketClientMemoria == -1)
		logError("No se pudo establecer la conexion con la memoria");
	else
		logInfo("Conexion establecida a traves del FD %d",
				cpuStruct->socketClientMemoria);
}

/**
 * Creo el socket client para conectar con kernel
 */
void createKernelClientSocket(cpu_struct* cpuStruct) {

	logInfo("Estableciendo conexion con kernel");

	cpuStruct->socketClientKernel = crearSocketCliente(cpuStruct->config->ip_kernel,
			cpuStruct->config->puerto_kernel);
	if (cpuStruct->socketClientKernel == -1)
		logError("No se pudo establecer la conexion con el file system");
	else
		logInfo("Conexion establecida a traves del FD %d", cpuStruct->socketClientKernel);
}
