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

	Package* package;
	logInfo("Estableciendo conexion con memoria");

	cpuStruct->socketClientMemoria = crearSocketCliente(
			cpuStruct->config->ip_memoria,
			cpuStruct->config->puerto_memoria);
	if (cpuStruct->socketClientMemoria == -1)
		logError("No se pudo establecer la conexion con la memoria");
	else{
		logInfo("Conexion establecida a traves del FD %d",
				cpuStruct->socketClientMemoria);
		logInfo("Esperando solicitud de identificación de la memoria");
		package = createAndReceivePackage(cpuStruct->socketClientMemoria);
		if(package != NULL && package->msgCode == COD_GET_IDENTIFICATION){
			destroyPackage(package);
			package = createAndSendPackage(cpuStruct->socketClientMemoria, COD_HANDSHAKE_CPU, 0 , NULL);
			if(package != NULL){
				destroyPackage(package);
				loadMemoryPageSize(cpuStruct); //con esta instruccion voy a solicitar el tamaño de pagina a la memoria
			} else {
				logError("No se pudo completar el handshake con la memoria");
				cpuStruct->socketClientMemoria = -1;
			}

		} else {
			logError("La memoria solicito una accion no permitida.");
			cpuStruct->socketClientMemoria = -1;
		}

	}
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
		logError("No se pudo establecer la conexion con el kernel");
	else
		logInfo("Conexion establecida a traves del FD %d", cpuStruct->socketClientKernel);
}
