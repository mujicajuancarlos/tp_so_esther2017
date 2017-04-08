/*
 * socket.h
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>

#include "configuration/configuration.h"


int crearSocketCliente(char* ip,int puerto);

#endif /* SOCKET_H_ */
