/*
 * socket.c
 *
 *  Created on: 7/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearConexiones(Configuration *config){

	int socketDesc, conexion;
	struct sockaddr_in direccionServerMemoria;
	socketDesc = socket(AF_INET, SOCK_STREAM, 0);
	direccionServerMemoria.sin_family = AF_INET;
	direccionServerMemoria.sin_addr.s_addr = inet_addr(config->ip_memoria);
	direccionServerMemoria.sin_port = htons(config->puerto_memoria);
	memset(&(direccionServerMemoria.sin_zero), '\0',8);

	if (socketDesc == -1){
		puts("Error en la creación del socket");
	}

	conexion = connect(socketDesc, (struct sockaddr *)&direccionServerMemoria, sizeof(struct sockaddr_in));
	if (conexion == -1){
		puts("No se pudo conectar con la Memoria");
	}


	return;
}
