/*
 * socket-server.c
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#include "socket-server.h"

int crearSocketServer(const char* ip, int port) {

	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
	direccionServidor.sin_port = htons(port);

	//creo el file descriptor
	int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(socketFileDescriptor == -1){
		error_show("Create socket file descriptor failed FD: %d address: %c port: %d",
								socketFileDescriptor, ip, port);
		return SOCKET_FAILURE;
	}

	int activado = 1;
	if(setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &activado,
			sizeof(activado)) != 0){
		error_show("Set socket option failed FD: %d address: %c port: %d",
						socketFileDescriptor, ip, port);
		return SOCKET_FAILURE;
	}

	if (bind(socketFileDescriptor, (void*) &direccionServidor,
			sizeof(direccionServidor)) != 0) {
		error_show("Socket bind failed FD: %d address: %c port: %d",
								socketFileDescriptor, ip, port);
		return SOCKET_FAILURE;
	}

	if (listen(socketFileDescriptor, 100) == -1) {
		error_show("Socket listen failed FD: %d address: %c port: %d",
										socketFileDescriptor, ip, port);
		close(socketFileDescriptor);
		return SOCKET_FAILURE;
	}

	return socketFileDescriptor;
}

int aceptarConexionCliente (int socketServerFileDescriptor){

	struct sockaddr cliente;
	socklen_t longitudCliente = sizeof (cliente);
	int newSocketFileDescriptor;

	newSocketFileDescriptor = accept (socketServerFileDescriptor, &cliente, &longitudCliente);

	if (newSocketFileDescriptor == -1){
		error_show("Socket accept failed FD: %d", socketServerFileDescriptor);
		return SOCKET_FAILURE;
	}

	return newSocketFileDescriptor;
}
