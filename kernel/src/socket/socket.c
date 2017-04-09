/*
 * socket.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "socket.h"

void crearSockets(kernel_struct *args) {

	//INADDR_ANY utilizado para obtener la ip local de forma automatica -> no estaria funcionando
	//TODO averiguar como obtener la ip local
	char* server_ip = "0.0.0.0"; //INADDR_ANY;

	//server socket para atender los pedidos de la consola
	args->socketServerConsola = abrirSocketInetServer(server_ip, args->config->puerto_program);
	if (args->socketServerConsola == -1){
		logError("No se pudo crear el server para las consolas");
		exit(-1);
	}
	logInfo("Server Socket de consolas esta escuchando");

	//server socket para atender los pedidos del cpu
	args->socketServerCPU = abrirSocketInetServer(server_ip, args->config->puerto_cpu);
	if (args->socketServerCPU == -1) {
		logError("No se pudo crear el server para cpu's");
		exit(-1);
	}
	logInfo("Server Socket de cpu's esta escuchando");
}

/**
 * Genera un socket Inet, recibe una ip y un puerto
 */
int abrirSocketInetServer(const char* ip, int port) {
	/*CPUs
	 * Se rellenan los campos de la estructura Direccion, necesaria
	 * para la llamada a la funcion bind()
	 */
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(ip);
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
