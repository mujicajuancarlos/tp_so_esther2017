/*
 * socket.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearConexiones(Configuration *config){

	int socketfd;
	char *msg = "Hola conexión nueva";

	socketfd = crearSocketServer(config->puerto);

	puts("Se enlazó al puerto");

	while(1){

		char *buffer = malloc(20);

		puts("Esperando al cliente...");
		int accepted = aceptarConexionCliente(socketfd);
		puts("Atendiendo al cliente...");

		int bytes_recv = recv(accepted, buffer, 20, 0);
		if (bytes_recv < 0){
			puts("Error al recibir ");
		}
		buffer[bytes_recv]='\0';
		printf("Me llegaron %d bytes con %s\n", bytes_recv, buffer);

		printf("%s\n",msg);
		puts("Te escuché");
		free(buffer);
		close(accepted);

	}

	return ;
}

