/*
 * cpu.h
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#ifndef CPU_CPU_H_
#define CPU_CPU_H_

#include <pthread.h>

#include "../kernel.h"
#include "../socket/client.h"
#include "../socket/server.h"

void handleCPUs(kernel_struct *args);
void analizarMensajeCPU(int socketDescriptor , Package* package, kernel_struct *args);
void nuevoCPU(t_list* listaCPUs, int socketCPU_fd);

#endif /* CPU_CPU_H_ */
