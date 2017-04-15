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

#define SALUDO_1 1
#define SALUDO_2 2
#define SALUDO_3 3

void handleCPUs(kernel_struct *args) {

	t_list* listaCPUs = args->listaCPUs;
	int socketServidor = args->socketServerCPU; // file descriptor del socket servidor

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

		for (i = 0; i < MAX_CPUS; i++) {

			socketClientDescriptor = (args->cpuSockets)[i];

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
			logError("Error al hacer el select del socket server de cpu");

		logInfo("Llego un nuevo pedido");

		// Verifico si algún cliente que ya estaba conectado ha enviado un mensaje
		for (i = 0; i < MAX_CPUS; i++) {
			socketClientDescriptor = (args->cpuSockets)[i];
			if (FD_ISSET(socketClientDescriptor, &descriptoresLectura)) {
				//creo un packete para almacenar la informacion recibida
				Package* package = createPackage();
				//identifico el mensaje enviado por el cliente
				if (recieve_and_deserialize(package, socketClientDescriptor)
						> 0) {
					//logDebug("CPU %d envía [message code]: %d, [Mensaje]: %s", i+1, package->msgCode, package->message);
					analizarMensajeCPU(socketClientDescriptor, package, args);
				} else {
					//si llego aca es porque debe cerrarse la conexion
					logInfo("CPU %d ha cerrado la conexión", i);
					//TODO descomentar y agregar la funcion para crear la estructura cpu y agregarla a listaCPUs
					//eliminarCPU(listaCPUs, socketClientDescriptor);
					(args->cpuSockets)[i] = 0;
				}
				destroyPackage(package);
			}
		}

		// Verifico si hay un nuevo cliente tratado de conectarse
		if (FD_ISSET(socketServidor, &descriptoresLectura)) {
			socketClientDescriptor = registrarNuevoCliente(socketServidor,
					args->cpuSockets, MAX_CPUS);
			if (socketClientDescriptor != -1) {
				nuevoCPU(listaCPUs, socketClientDescriptor);
			}
		}

	}

}

void nuevoCPU(t_list* listaCPUs, int socketCPU_fd) {
	//este es el momento que deberia crearse una nueva cpu
	//TODO a completar
	//crear una nueva cpu, responder adecuadamente al cliente para comenzar el proceso
	char* message = "Hola, te acabas de conectar al servidor kernel";
	if (send(socketCPU_fd, message, strlen(message), 0) != strlen(message)) {
		logError("No se pudo enviar el mensaje al nuevo cliente conectado");
	}
	logInfo("Se envio la respuesta de conexion para el cliente %d",
			socketCPU_fd);
}

/**
 * Esta funcion es invocada cuando se recibe un mensaje de un cliente cpu que ya establecio conexion con el kernel
 */
void analizarMensajeCPU(int socketDescriptor, Package* package,
		kernel_struct *args) {
	if (package->msgCode == SALUDO_1) {
		logTrace("CPU %d me informa que debo ejecutar el saludo 1",
				socketDescriptor);
		printf("Saludo numero 1");
	}
	char* message = "Este es mensaje por defecto, para avisarte que me llego tu solicitud";
	if (send(socketDescriptor, message, strlen(message), 0) != strlen(message)) {
			logError("No se pudo enviar el mensaje al nuevo cliente conectado");
		}
		logInfo("Se envio la respuesta al cliente %d", socketDescriptor);
}
