/*
 * socket.c
 *
 *  Created on: 6/4/2017
 *      Author: utnso
 */
#include "socket.h"

int crearSocketCliente(char* ip,int puerto){


	/*creo y completo estructura para el connect */
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
	direccionServidor.sin_port = htons(puerto);
	memset(&(direccionServidor.sin_zero), '\0', 8);

	/* socket y connect */

	int socketCreado = socket(AF_INET,SOCK_STREAM,0);
	if(socketCreado == -1){
		puts("Error al crear socket");
	}

	int conectando = connect(socketCreado, (struct sockaddr *)& direccionServidor,sizeof(struct sockaddr_in));
	if(conectando == -1){
		puts("Error en el connect");
	}

	return socketCreado;

}


