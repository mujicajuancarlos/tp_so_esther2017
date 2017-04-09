/*
 * socket.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef SOCKET_SOCKET_H_
#define SOCKET_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <bits/socket.h>
#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

//commoms
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

#include "../configuration/configuration.h"
#include "../kernel.h"
/**
 * DEFINICION DE FUNCIONES
 */

void crearSockets(kernel_struct* args);
int abrirSocketInetServer(const char* ip, int port);

#endif /* SOCKET_SOCKET_H_ */
