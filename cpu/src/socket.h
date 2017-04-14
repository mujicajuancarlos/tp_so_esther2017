/*
 * socket.h
 *
 *  Created on: 7/4/2017
 *      Author: utnso
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>

#include <dc-commons/socket-clien.h>
#include "configuration.h"

void crearConexiones(Configuration* config);

#endif /* SOCKET_H_ */
