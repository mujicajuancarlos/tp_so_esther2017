/*
 * handler-kernel.c
 *
 *  Created on: 15/4/2017
 *      Author: utnso
 */

#include "handler-kernel.h"

void handleKernel(cpu_struct *cpuStruct) {

	Package* package;

	while (true) {

		/*
		 * Quedo a la espera de REQUEST por parte del Kernel
		 */
		package = createAndReceivePackage(cpuStruct->socketClientKernel);

		if (package != NULL)
			handleKernelRequest(cpuStruct, package);
		else
			logError("El kernel cerro la conexion para FD: %d", cpuStruct->socketClientKernel);

		destroyPackage(package);
	}
}

void handleKernelRequest(cpu_struct *args, Package *package) {

	switch (package->msgCode) {
	case COD_CPU_RUN_PCB:
		/*
		 * TODO implementar la ejecucion de de un PCB
		 *
		 *cuando soliciten informacion a la memoria se debe usar funciones implementadas en module memory
		 *No conviene usar hilos en esta parte porque las operaciones siempre son sincronizadas,
		 * EJ:	inicio ejecucion
		 * 		solicito "cosas a la memoria"
		 * 		la memoria me responde
		 * 		ejecuto lo que tenga que ejecutar
		 * 		devuelvo al kernel el nuevo pcb
		 *
		 */
		break;
		/**
		 * TODO implementar el el resto de las peticiones
		 */
	default:
		logError("El kernel solicito una accion no permitida");
		break;
	}
}
