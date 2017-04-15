/*
 * console.c
 *
 *  Created on: 10/4/2017
 *      Author: utnso
 */

#include "handler-console.h"

void handleConsolas(kernel_struct *args) {

	t_list* listaConsolas = args->listaConsolas;
	int socketServidor = args->socketServerConsola; // file descriptor del socket servidor

	fd_set descriptoresLectura; //defino el descriptor de lectura
	int i;
	int activity;
	int socketClientDescriptor;
	int max_socketDescriptor = socketServidor;

	/* Bucle infinito.
	 * Se atiende a si hay más clientes para conectar y a los mensajes enviados
	 * por los clientes ya conectados */
	while (1) {

		//borra el set de descriptiores
		FD_ZERO(&descriptoresLectura);
		//relaciona los descriptores con el socket server de cpu's
		FD_SET(socketServidor, &descriptoresLectura);

		//add child sockets to set

		for (i = 0; i < MAX_CONSOLAS; i++) {

			socketClientDescriptor = (args->consolaSockets)[i];

			//si es un descriptor valido entonces lo agrego al set para que pueda leer paquetes
			if (socketClientDescriptor > 0)
				FD_SET(socketClientDescriptor, &descriptoresLectura);

			//si el nuevo socketDescriptos es mayor al que tenia lo actualizo
			//Esto es necesario para la funcion select
			if (socketClientDescriptor > max_socketDescriptor)
				max_socketDescriptor = socketClientDescriptor;
		}

		logInfo("Esperando nuevas conexiones y mensajes");
		/* Espera indefinida hasta que alguno de los descriptores tenga algo
		 * que decir -> puede ser producido por:
		 * 1. Nuevo cliente solicitando conectarse
		 * 2. Un cliente ya conectado que envía un mensaje*/
		activity = select(max_socketDescriptor + 1, &descriptoresLectura,
		NULL, NULL,
		NULL);
		if ((activity < 0) && (errno != EINTR))
			logError("Error al hacer el select del socket server de consola");

		// Verifico si algún cliente que ya estaba conectado ha enviado un mensaje
		for (i = 0; i < MAX_CONSOLAS; i++) {
			socketClientDescriptor = (args->consolaSockets)[i];
			if (FD_ISSET(socketClientDescriptor, &descriptoresLectura)) {
				//creo un packete para almacenar la informacion recibida
				Package* package = createEmptyPackage();
				if (receivePackage(socketClientDescriptor, package) == 0)
					handleConsoleRequest(socketClientDescriptor, package, args);
				else
					logError("Error al intentar recibir los datos del FD: %D",
							socketClientDescriptor);
				destroyPackage(package);
			}
		}

		// Verifico si hay un nuevo cliente tratado de conectarse
		if (FD_ISSET(socketServidor, &descriptoresLectura)) {
			socketClientDescriptor = registrarFileDescriptorInArray(
					socketServidor, args->consolaSockets, MAX_CONSOLAS);
			if (socketClientDescriptor != -1) {
				nuevaConsola(listaConsolas, socketClientDescriptor);
			}
		}

	}

}

void handleConsoleRequest(int fileDescriptor, Package *package,
		kernel_struct *args) {
	switch (package->msgCode) {
	case COD_INICIAR_PROGRAMA:
		logInfo("Ejecutando inicio de programa");
		break;
	case COD_FINALIZAR_PROGRAMA:
		logInfo("Ejecutando finalizacion de programa");
		break;
	case COD_SALUDO:
		logInfo("La consola %d me envio el siguiente saludo: %s",
				fileDescriptor, package->message);
		logInfo("Replicando mensaje en cpus");
		void _send_message(void* element) {
			if (sendPackage(element, package) == -1)
				logError("No se pudo enviar el mensaje al cpu fd: %d", element);
			else
				logInfo("Se envió el mensaje a la cpu fd: %d", element);
		}
		list_iterate(args->listaCPUs, _send_message);
		if (args->socketClientFileSystem != 0) {
			if (sendPackage(args->socketClientFileSystem, package) == -1)
				logError("No se pudo enviar el mensaje al file system fd: %d",
						args->socketClientFileSystem);
			else
				logInfo("Se envió el mensaje a la file system fd: %d",
						args->socketClientFileSystem);
		}
		if (args->socketClientMemoria != 0) {
			if (sendPackage(args->socketClientMemoria, package) == -1)
				logError("No se pudo enviar el mensaje a la memoria fd: %d",
						args->socketClientMemoria);
			else
				logInfo("Se envió el mensaje a la memoria fd: %d",
						args->socketClientMemoria);
		}
		break;
	default:
		logError("La consola solicito una accion no permitida");
		break;
	}

}

void crearServerSocketParaConsola(kernel_struct* args) {
	//server socket para atender los pedidos de la consola
	args->socketServerConsola = crearSocketServer(args->config->puerto_program);
	if (args->socketServerConsola == -1) {
		logError("No se pudo crear el server para las consolas");
		exit(-1);
	}
	logInfo("Server Socket de consolas esta escuchando");
	inicializarArray(MAX_CONSOLAS, args->consolaSockets);
}

void nuevaConsola(t_list* listaConsolas, int socketConsola_fd) {
	//este es el momento que deberia crearse una nueva cpu
	//TODO a completar
	//crear una nueva cpu, responder adecuadamente al cliente para comenzar el proceso
	char* message = "Hola, te acabas de conectar al servidor kernel";
	if (send(socketConsola_fd, message, strlen(message), 0)
			!= strlen(message)) {
		logError("No se pudo enviar el mensaje al nuevo cliente conectado");
	}
	logInfo("Se envio la respuesta de conexion para el cliente %d",
			socketConsola_fd);
}
