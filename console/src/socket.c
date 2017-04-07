/*
 * socket.c
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */
#include "socket.h"
void crearSocket(Configuration* config){


	/*creo y completo estructura para el connect */
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(config->ip_kernel);
	direccionServidor.sin_port = htons(config->puerto_kernel);
	memset(&(direccionServidor.sin_zero), '\0', 8);

	/* socket y connect */

	int socketCreado = socket(AF_INET,SOCK_STREAM,0);

	connect(socketCreado, (struct sockaddr *)& direccionServidor,sizeof(struct sockaddr_in));

	return;

}


