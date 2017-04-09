/*
 * socket.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearConexiones(Configuration *config){

	int socketDesc, binder;
	struct sockaddr_in my_addr;

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
		puts("Error al asociar el proceso en un puerto");
	}


}
