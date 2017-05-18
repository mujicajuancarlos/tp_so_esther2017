/*
 * handler-planning.c
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#include "handler-planning.h"
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <dc-commons/src/dc-commons/protocol-kernel-cpu.h>
#include "processLifeCycle.h"

pthread_mutex_t executeListMutex;

t_planningStates* states;

PCB* removeFromEXEC(int pid){
	int i;
	bool encontrado = false;
	pthread_mutex_lock(&executeListMutex);
	t_list* enEjecucion = states->execute;
	PCB* proceso = NULL;
	for( i=0; i < enEjecucion->elements_count; i++){
		proceso = list_get(enEjecucion,i);
		//saca de la lista y retorna el proceso cuando lo encuentra por el ID
		if(proceso->pid==pid){
			list_remove(enEjecucion,i);
			encontrado = true;
			break;
		}
	}
	pthread_mutex_unlock(&executeListMutex);
	logTrace("Plan: PCB:%d / EXEC ->",pid);
	if(encontrado){
		return proceso;
	} else {
		return NULL;
	}
}
void actualizarPCB(PCB* local, PCB* actualizado){
	local->programCounter = actualizado->programCounter;
	local->stackOffset = actualizado->stackOffset;
	t_stack_program* contexto_aux = local->stackSize;
	/*uint32_t context_len_aux = local->context_len;
	local->context_len = actualizado->context_len;
	actualizado->context_len = context_len_aux;
	*/
	local->stackSize = actualizado->stackSize;
	actualizado->stackSize = contexto_aux;
}
void sendToEXIT(PCB* pcb){
	pthread_mutex_lock(&exitListMutex);
	queue_push(states->exit,pcb);
	pthread_mutex_unlock(&exitListMutex);
	logTrace("Plan: PCB:%d / -> EXIT",pcb->pid);
}

void informarPlanificadorFinalizarPrograma(){
	//enviarMensajeSocket(socketPlanificador,FINALIZAR_PROGRAMA,"");
}

void finalizarPrograma(PCB* pcbActualizado, int socketCPU){
		PCB* proceso = removeFromEXEC(pcbActualizado->pid);
		actualizarPCB(proceso,pcbActualizado);
		destroy_PCB(pcbActualizado);
		sendToEXIT(proceso);
		//informarPlanificadorFinalizarPrograma();
	}

}
