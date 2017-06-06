/*
 * sharedSemaphore.c
 *
 *  Created on: 6/6/2017
 *      Author: utnso
 */

#include "sharedSemaphore.h"

bool executeBasicWait(t_nombre_semaforo semId) {
	/*int pos = getPosicionSemaforo(semId);
	sem_values[pos]--;
	logDebug("[WAIT] Semaforo %s. Valor actual: %d", semId, sem_values[pos]);

	return sem_values[pos] < 0;*/
	return false;
}

void executeBasicSignal(t_nombre_semaforo semId) {
/*
	int pos = getPosicionSemaforo(semId);
	sem_values[pos]++;
	logDebug("[SIGNAL] Semaforo %s. Valor actual: %d",semId,sem_values[pos]);
	if(sem_values[pos]<=0){
		PCB* pcb = getNextFromSemaforo(pos);
		logDebug("Desbloqueado de Semaforo %s, PID: %d",semId,pcb->processID);
		if(pcb!=NULL){
			if(consola_desconectada(pcb->consolaFD)){
				sendToEXIT(pcb);
				informarPlanificadorFinalizarPrograma();
			} else {
				notifyProcessREADY(pcb);
			}
		}
	}*/

}
