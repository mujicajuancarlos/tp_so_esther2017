/*
 * socket.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearConexiones(Configuration *config){

	int socketDesc, binder, listener, new_descriptor;
	struct sockaddr_in my_addr, remote_addr;

	socketDesc = socket(AF_INET, SOCK_STREAM, 0);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = 0;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);

	if (socketDesc == -1){
		puts("Error en la creación del socket");
	}

	binder = bind(socketDesc, (struct sockaddr *)my_addr, sizeof(struct sockaddr));

	if (binder == -1){
		puts("Error al asociar el proceso a un puerto");
	}

	listener = listen(socketDesc,10);

	if (listener == -1){
		puts("Error al esperar conexiones de entrada");
	}

	int sin_size = sizeof(struct sockaddr_in);
	new_descriptor = accept(socketDesc, (struct sockaddr *)&remote_addr, &sin_size);

	if (new_descriptor == -1){
		puts("Error al crear receptor de llamadas");
	}
}
