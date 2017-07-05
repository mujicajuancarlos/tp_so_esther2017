/*
 * handler-planning.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-planning.h"

pthread_mutex_t mutexP;
void handlePlanning(kernel_struct *kernel_struct) {
	CPU* cpu;
	int algorithm;
	while (true) {
		if(!isEnableShortTermScheduler()){
			logInfo("El planificador de corto plazo se encuentra bloqueado por la consola del kernel");
			shortTermSchedulerMutex_lock();
		}
		cpu = searchAndMarkBusyCPU();
		algorithm = getAlgorithmIndex(kernel_struct->config->algoritmo);
		switch (algorithm) {
		case ROUND_ROBIN:
			execute_RoundRobin_scheduling(kernel_struct, cpu);
			break;
		case FIFO:
			execute_FIFO_scheduling(kernel_struct, cpu);
			break;
		default:
			logError("El algoritmo de planificacion ingresado en el archivo de configuracion no es valido");
			exit (-1);
			break;
		}
	}

}
