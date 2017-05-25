/*
 * handler-planning.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-planning.h"

pthread_mutex_t mutexP;
void handlePlanning(kernel_struct *kernel_struct) {

	while (true) {
		shortTermScheduler_lock();
		/**1. mutex de habilitacion del planificador
		 * 2. wait del semaforo de cpus disponibles + obtener la instancia de cpu disponible
		 * 		CPU* cpu = searchAndMarkBusyCPU()
		 * 3. wait del semaforo de procesos en ready
		 * 4. solicito al algoritmo que me de el siguiente proceso ready (sacar el proceso de la lista ready)
		 * 5. enviar a ejecutar el proceso seleccionado por el algoritmo a la cpu seleccionada
		 */
		shortTermScheduler_unlock();
	}

}
