/*
 * socket.c
 *
 *  Created on: 7/4/2017
 *      Author: utnso
 */

#include "socket.h"

int conectarAMemory(char* ip,int puerto){

	int socketClient, connector;
	struct sockaddr_in direccionServerMemoria;


	direccionServerMemoria.sin_family = AF_INET;
	direccionServerMemoria.sin_addr.s_addr = inet_addr(ip);
	direccionServerMemoria.sin_port = htons(puerto);
	memset(&(direccionServerMemoria.sin_zero), '\0',8);

	socketClient = socket(AF_INET, SOCK_STREAM, 0);

	connector = connect(socketClient, (void *)&direccionServerMemoria, sizeof(direccionServerMemoria));
	if ( connector == -1){
		puts("No se pudo conectar...");
	}

	return socketClient;
}
