/*
 * socket.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef SOCKET_SOCKET_H_
#define SOCKET_SOCKET_H_

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../configuration/configuration.h"
/**
 * DEFINICION DE FUNCIONES
 */

void crearSockets(Configuration* config);
int abrirSocketInetServer(const char* ip, int port);

#endif /* SOCKET_SOCKET_H_ */
