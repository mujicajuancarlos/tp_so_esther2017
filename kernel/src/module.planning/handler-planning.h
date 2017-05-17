/*
 * handler-planning.h
 *
 *  Created on: 29/4/2017
 *      Author: utnso
 */

#ifndef MODULE_PLANNING_HANDLER_PLANNING_H_
#define MODULE_PLANNING_HANDLER_PLANNING_H_
#include "../kernelStruct.h"
//no me reconoce la PCB que esta declarada en las commons
#include <dc-commons/src/dc-commons/protocol-kernel-cpu.h>
#include "processLifeCycle.h"


//Funciones
void sendToEXIT(PCB* pcb);
PCB* removeFromEXEC(int pid);
void actualizarPCB(PCB* local, PCB* actualizado);
void informarPlanificadorFinalizarPrograma();
void finalizarPrograma(PCB* pcb, int socketCPU);

#endif /* MODULE_PLANNING_HANDLER_PLANNING_H_ */
