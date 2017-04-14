/*
 * socket.c
 *
 *  Created on: 7/4/2017
 *      Author: utnso
 */

#include "socket.h"

int conectarAMemory(char* ip,int puerto){

	int socketClient;

	socketClient = crearSocketCliente(ip,puerto);

	return socketClient;
}
