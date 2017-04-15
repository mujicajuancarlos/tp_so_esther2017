/*
 * cpu.h
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#ifndef HANDLER_CPU_H_
#define HANDLER_CPU_H_

#include <dc-commons/package.h>
#include <dc-commons/socket-message.h>
#include <dc-commons/protocol-kernel-cpu.h>

#include "kernel.h"
#include "commonFunctions.h"

void handleCPUs(kernel_struct *args);
void handleCpuRequest(int fileDescriptor, Package *package);
void crearServerSocketParaCpus(kernel_struct* args);
void crear_registrar_CPU(t_list* listaCPUs, int socketCPU_fd);

#endif /* HANDLER_CPU_H_ */
