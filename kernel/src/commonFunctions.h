/*
 * socket.h
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#ifndef COMMONFUNCTIONS_H_
#define COMMONFUNCTIONS_H_

#include <dc-commons/socket-server.h>

#include "kernel.h"
#include "handler-console.h"
#include "handler-cpu.h"
/**
 * DEFINICION DE FUNCIONES
 */

/**
 * funcion que genera los sockets cliente y servidor para el proceso kernel
 */
void crearSockets(kernel_struct* args);

/**
 * funcion que recibe un array y su tamaño para inicializar con -1
 */
void inicializarArray(int tamanio_maximo, int* arraySockets);

/**
 * funcion que setea el file descriptor en la primer posicion disponible que encuentre
 */
int registrarFileDescriptorInArray(int fd_socketServer, int* arraySockets, int max_clientes);

#endif /* COMMONFUNCTIONS_H_ */
