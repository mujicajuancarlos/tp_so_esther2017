/*
 * socket-client.c
 *
 *  Created on: 12/4/2017
 *      Author: utnso
 */

#include "socket-client.h"

int crearSocketCliente(const char* ip, int port) {
	int socketClientFileDescription;
	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(port);

	socketClientFileDescription = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketClientFileDescription == -1)
		error_show("Create file descriptor failed FD: %d address: %c port: %d",
				socketClientFileDescription, ip, port);
	return SOCKET_FAILURE;

	if (connect(socketClientFileDescription, (void*) &serv_addr,
			sizeof(serv_addr)) == -1) {
		error_show("Connect failed FD: %d address: %c port: %d",
				socketClientFileDescription, ip, port);
		return SOCKET_FAILURE;
	}

	return socketClientFileDescription;
}
