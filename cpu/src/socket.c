/*
 * socket.c
 *
 *  Created on: 7/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearConexiones(Configuration *config){

	int socketCreado;
	struct sockaddr_in direccionServerMemoria;
	socketCreado = socket(AF_INET, SOCK_STREAM, 0);
	direccionServerMemoria.sin_family = AF_INET;
	direccionServerMemoria.sin_addr.s_addr = inet_addr(config->ip_memoria);
	direccionServerMemoria.sin_port = htons(config->puerto_memoria);
	memset(&(direccionServerMemoria.sin_zero), '\0',8);

	connect(socketCreado, (struct sockaddr *)&direccionServerMemoria, sizeof(struct sockaddr_in));

	return;
}
