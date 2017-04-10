/*
 * console.h
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#ifndef CONSOLE_CONSOLE_H_
#define CONSOLE_CONSOLE_H_

#include <pthread.h>

#include "../kernel.h"
#include "../socket/client.h"
#include "../socket/server.h"

void handleConsolas(kernel_struct *args);
void analizarMensajeConsola(int socketDescriptor , Package* package, kernel_struct *args);
void nuevaConsola(t_list* listaCPUs, int socketCPU_fd);



#endif /* CONSOLE_CONSOLE_H_ */
