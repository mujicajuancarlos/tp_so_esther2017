/*
 * socket.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "commonFunctions.h"




void inicializarArray(int tamanio_maximo, int* arraySockets) {
	int i;
	for (i = 0; i < tamanio_maximo; i++) {
		arraySockets[i] = -1;
	}
}

// registra el socket_fd y lo devuelve, si devuelve -1 es porque no lo pudo registrar
int registrarFileDescriptorInArray(int fd_socketServer, int* arraySockets, int max_clientes) {

	int new_socket_fd = aceptarConexionCliente(fd_socketServer);
	int estadoRegistrado = -1;
	int i;

	//Se acepto la conexion por lo tanto se tiene que guardar en el array de conexiones
	for (i = 0; i < max_clientes; i++) {
		//si la posicion no esta ocupada entonces registo el socket del cliente
		if (arraySockets[i] == -1) {
			arraySockets[i] = new_socket_fd;
			estadoRegistrado = i;
			logInfo("Se registro la nueva conexion del server socket %d tu id es %d",fd_socketServer,i);
			break;
		}
	}
	//si el estado de registro es -1 es porque se llego al limite de conexiones permitidas y no se pudo registrar
	if(estadoRegistrado == -1){
		logTrace("La nueva conexion no pudo aceptarse porque el servidor llego a su limite permitido");
		close(new_socket_fd);
		return estadoRegistrado;
	}
	return new_socket_fd;
}
