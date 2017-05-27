/*
 * kernelRequest.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#include "kernelRequest.h"

void executeNewPcb(cpu_struct* cpuStruct, Package* package){
	PCB* pcb = deserialize_PCB(package->stream);
	loadPCB(pcb);
	logTrace("El PCB para pid: %d fue cargado", pcb->pid);
	ansisopExecuteInstruccion(cpuStruct);
	logTrace("Rafaga ejecutada para el pid: %d", pcb->pid);
}

void continueExecution(cpu_struct* cpuStruct, Package* package){
	logTrace("Continuando la ejecucion de la siguiente instruccion ansisop");
	ansisopExecuteInstruccion(cpuStruct);

}

void contextSwitch(cpu_struct* cpuStruct, Package* package){
	logTrace("Descargando el actual pcb");
	unloadPCB(cpuStruct);
}
