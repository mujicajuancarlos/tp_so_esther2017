/*
 * socket.c
 *
 *  Created on: 5/4/2017
 *      Author: utnso
 */

#include "server.h"

void crearSockets(kernel_struct *args) {

	//INADDR_ANY no funciono, por eso uso 0.0.0.0 que tiene el mismo efecto
	char* server_ip = "0.0.0.0"; //INADDR_ANY;

	//server socket para atender los pedidos de la consola
	crearServerSocketParaConsola(args, server_ip);

	//server socket para atender los pedidos del cpu
	crearServerSocketParaCpus(args, server_ip);
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

void crearServerSocketParaConsola(kernel_struct* args, char* server_ip) {
	//server socket para atender los pedidos de la consola
	args->socketServerConsola = abrirSocketInetServer(server_ip,
			args->config->puerto_program);
	if (args->socketServerConsola == -1) {
		logError("No se pudo crear el server para las consolas");
		exit(-1);
	}
	logInfo("Server Socket de consolas esta escuchando");
	inicializarSockets(MAX_CONSOLAS, args->consolaSockets);
}

void crearServerSocketParaCpus(kernel_struct* args, char* server_ip) {
	//server socket para atender los pedidos del cpu
	args->socketServerCPU = abrirSocketInetServer(server_ip,
			args->config->puerto_cpu);
	if (args->socketServerCPU == -1) {
		logError("No se pudo crear el server para cpu's");
		exit(-1);
	}
	logInfo("Server Socket de cpu's esta escuchando");
	inicializarSockets(MAX_CPUS, args->cpuSockets);
}

void inicializarSockets(int tamanio_maximo, int* arraySockets) {
	int i;
	for (i = 0; i < tamanio_maximo; i++) {
		arraySockets[i] = -1;
	}
}

int recieve_and_deserialize(Package *package, int socketCliente) {

	int leidos = 0;
	int aux = 0;

	int buffer_size;
	char *buffer = malloc(buffer_size = sizeof(uint32_t));

	aux = recv(socketCliente, buffer, sizeof(package->msgCode), 0);
	memcpy(&(package->msgCode), buffer, buffer_size);
	if (!aux)
		return 0;
	leidos += aux;

	aux = recv(socketCliente, buffer, sizeof(package->message_long), 0);
	memcpy(&(package->message_long), buffer, buffer_size);
	if (!aux)
		return 0;
	leidos += aux;

	package->message = malloc(sizeof(char) * package->message_long);
	aux = recv(socketCliente, package->message, package->message_long, 0);
	if (!aux)
		return 0;
	leidos += aux;

	free(buffer);

	return leidos;
}

// registra el socket_fd y lo devuelve, si devuelve -1 es porque no lo pudo registrar
int registrarNuevoCliente(int fd_socketServer, int* arraySockets, int max_clientes) {

	socklen_t longitudCliente;
	struct sockaddr clienteAddr;
	int new_socket_fd;
	int estadoRegistrado = -1;
	int i;

	//como es el primer pedido el tamaño es fijo, osea el tamaño de la estructura socket address
	longitudCliente = sizeof(clienteAddr);
	if ((new_socket_fd = accept(fd_socketServer, &clienteAddr, &longitudCliente))
			< 0) {
		logError("Error al aceptar una nueva conexion");
		exit(EXIT_FAILURE);
	};
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
