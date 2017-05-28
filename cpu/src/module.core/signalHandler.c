/*
 * signalHandler.c
 *
 *  Created on: 28/5/2017
 *      Author: utnso
 */

#include "signalHandler.h"

void signal_handler(int signal) {
	switch (signal) {
		case SIGUSR1:
			logInfo("Se recibio la señal SIGUSR1");
			finalize_CPU_process();
			break;
		default:
			logError("Se recibio una señal no controlada por el proceso");
			break;
	}
}
