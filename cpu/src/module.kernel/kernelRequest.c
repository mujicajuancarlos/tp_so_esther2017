/*
 * kernelRequest.c
 *
 *  Created on: 27/5/2017
 *      Author: utnso
 */

#include "kernelRequest.h"

void updateStackPageSize(cpu_struct* cpuStruct, Package* package) {
	int size = deserialize_int(package->stream);
	logInfo(
			"El kernel indico que la cantidad de paginas permitidas para el stack es %d",
			size);
	setStackPageSize(size);
}

void executeNewPcb(cpu_struct* cpuStruct, Package* package) {
	PCB* pcb = deserialize_PCB(package->stream);
	if (pcb->stackIndex == NULL)
		createNewContext(pcb);
	loadPCB(pcb);
	logTrace("El PCB para pid: %d fue cargado", pcb->pid);
	ansisopExecuteInstruccion(cpuStruct);
	logTrace("Se ejecuto la instruccion ansisop del pid: %d", pcb->pid);
}

void continueExecution(cpu_struct* cpuStruct, Package* package) {
	logTrace("Continuando la ejecucion de la siguiente instruccion ansisop");
	ansisopExecuteInstruccion(cpuStruct);
	logTrace("Se ejecuto la instruccion ansisop del pid: %d", getPCB()->pid);
}

void contextSwitch(cpu_struct* cpuStruct, Package* package) {
	logTrace("Descargando el actual pcb");
	unloadPCB(cpuStruct);
}
