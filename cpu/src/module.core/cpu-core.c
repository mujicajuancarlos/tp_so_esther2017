/*
 * cpu-core.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#include "cpu-core.h"

PCB* pcb;

void loadPCB(PCB* newPCB){
	pcb = newPCB;
	logTrace("Contexto de proceso cargado PID:%d...",pcb->pid);
}
