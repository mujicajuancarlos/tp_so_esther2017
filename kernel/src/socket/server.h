/*
 * socket.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_

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

#include "package.h"
#include "../configuration/configuration.h"
#include "../kernel.h"
/**
 * DEFINICION DE FUNCIONES
 */

void crearSockets(kernel_struct* args);
int abrirSocketInetServer(const char* ip, int port);
void crearServerSocketParaConsola(kernel_struct* args, char* server_ip);
void crearServerSocketParaCpus(kernel_struct* args, char* server_ip);
void inicializarSockets(int tamanio_maximo, int* arraySockets);
int recieve_and_deserialize(Package *package, int socketCliente);
int registrarNuevoCliente (int fd_socketServer, int* arraySockets, int max_clientes);

#endif /* SOCKET_SERVER_H_ */
