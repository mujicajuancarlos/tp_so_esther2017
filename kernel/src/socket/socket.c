/*
 * socket.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearSockets(Configuration* config) {

	char* server_ip = INADDR_ANY;

	//abrir server para escuchar CPUs
	int socketServerCPU = abrirSocketInetServer(server_ip,
			config->puerto_program);
	if (socketServerCPU == -1) {
		perror("Error al abrir servidor para CPUs");
		exit(-1);
	}
}

/**
 * Genera un socket Inet, recibe una ip y un puerto
 */
int abrirSocketInetServer(const char* ip, int port) {
	/*
	 * Se rellenan los campos de la estructura Direccion, necesaria
	 * para la llamada a la funcion bind()
	 */
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("0.0.0.0");
	direccionServidor.sin_port = htons(port);

	//se abre el socket
	int servidorFD = socket(AF_INET, SOCK_STREAM, 0);

	int activado = 1;
	setsockopt(servidorFD, SOL_SOCKET, SO_REUSEADDR, &activado,
			sizeof(activado));

	if (bind(servidorFD, (void*) &direccionServidor, sizeof(direccionServidor))
			!= 0) {
		perror("Fallo al hacer el bind (Server)\n");
		return 1;
	}
	printf("Estoy escuchando\n");
	/*
	 * Se avisa al sistema que comience a atender llamadas de clientes
	 */
	if (listen(servidorFD, 100) == -1) {
		close(servidorFD);
		return -1;
	}
	/*
	 * Se devuelve el descriptor del socket servidor
	 */
	return servidorFD;
}
