/*
 ============================================================================
 Name        : socket.c
 Author      : JuanCarlosMujica
 Version     :
 Copyright   : UTN
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include "configuration.h"

void crearConexiones(Configuration* config);

#endif /* SOCKET_H_ */