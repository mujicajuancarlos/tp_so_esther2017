/*
 * cpu.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include "cpu.h"
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handleCPUs(kernel_struct *args) {

	t_list* listaCPUs = args->listaCPUs;
	int socketServidor = args->socketServerCPU; // Descriptor del socket servidor

	fd_set descriptoresLectura;	//variable que define el set de descriptores de sockets
	int clientesConectados = 0; /* Número clientes conectados */

	//int *socketCliente = args->cpuSockets;/* Descriptores de sockets con clientes */

	//int buffer;							/* Buffer para leer de los socket */
	//int maximo;							/* Número de descriptor más grande */
	//int i;								/* Para bubles */

	/* Bucle infinito.
	 * Se atiende a si hay más clientes para conectar y a los mensajes enviados
	 * por los clientes ya conectados */
	while (1) {
		//borra el set de descriptiores
		FD_ZERO(&descriptoresLectura);

		//relaciona los descriptores con el socket server de cpu's
		FD_SET(socketServidor, &descriptoresLectura);

		/* Espera indefinida hasta que alguno de los descriptores tenga algo
		 * que decir: un nuevo cliente o un cliente ya conectado que envía un
		 * mensaje */
		int activity = select(socketServidor + 1, &descriptoresLectura, NULL,
				NULL, NULL);
		if ((activity < 0) && (errno != EINTR)) {
			printf("select error");
		}
		/* Se comprueba si algún cliente nuevo desea conectarse y se le
		 * admite */
		// verifica si hay algun cliente nuevo intentando conectarse
		int new_socket;
		struct sockaddr_in address;
		int addrlen = sizeof(address);
		char* message = "Gracias por conectarte al server para cpu!";
		if (FD_ISSET(socketServidor, &descriptoresLectura)) {
			if ((new_socket = accept(socketServidor,
					(struct sockaddr *) &address, (socklen_t*) &addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			printf(
					"New connection , socket fd is %d , ip is : %s , port : %d \n",
					new_socket, inet_ntoa(address.sin_addr),
					ntohs(address.sin_port));

			//send new connection greeting message
			if (send(new_socket, message, strlen(message), 0)
					!= strlen(message)) {
				perror("send");
			}

			puts("Welcome message sent successfully");

		}
	}

}
