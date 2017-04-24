/*
 * cpu.c
 *
 *  Created on: 9/4/2017
 *      Author: utnso
 */

#include "handler-cpu.h"

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

		// Verifico si algún cliente que ya estaba conectado ha enviado un mensaje
		for (i = 0; i < MAX_CPUS; i++) {
			socketClientDescriptor = (args->cpuSockets)[i];
			if (FD_ISSET(socketClientDescriptor, &descriptoresLectura)) {
				//creo un packete para almacenar la informacion recibida
				Package* package = createEmptyPackage();
				if (receivePackage(socketClientDescriptor, package) == 0)
					handleCpuRequest(socketClientDescriptor, package);
				else
					logError("Error al intentar recibir los datos del FD: %d",
							socketClientDescriptor);
				destroyPackage(package);
			}
		}

		// Verifico si hay un nuevo cliente tratado de conectarse
		if (FD_ISSET(socketServidor, &descriptoresLectura)) {
			socketClientDescriptor = registrarFileDescriptorInArray(
					socketServidor, args->cpuSockets, MAX_CPUS);
			if (socketClientDescriptor != -1) {
				crear_registrar_CPU(listaCPUs, socketClientDescriptor);
			}
		}

	}

}

void handleCpuRequest(int fileDescriptor, Package *package) {
	switch (package->msgCode) {
	case COD_ANSISOP_IMPRIMIR:
		logInfo("Ejecutando syscall imprimir");
		break;
	case COD_ANSISOP_SARAZA:
		logInfo("Ejecutando syscall saraza");
		break;
	case COD_SALUDO:
		logInfo("La cpu %d me envio el siguiente saludo: %s", fileDescriptor,
				package->stream);
		break;
	default:
		logError("La cpu solicito una accion no permitida");
		break;
	}
}

void crearServerSocketParaCpus(kernel_struct* args) {
	//server socket para atender los pedidos del cpu
	args->socketServerCPU = crearSocketServer(args->config->puerto_cpu);
	if (args->socketServerCPU == -1) {
		logError("No se pudo crear el server para cpu's");
		exit(-1);
	}
	logInfo("Server Socket de cpu's esta escuchando");
}

void crear_registrar_CPU(t_list* listaCPUs, int socketCPU_fd) {
	char* message = "Conexion con kernel establecida con kernel";
	Package* package = createPackage(COD_SALUDO, strlen(message), message);
	sendPackage(socketCPU_fd,package);
	CPU* cpu = malloc(sizeof(CPU));
	cpu->cpuFD = socketCPU_fd;
	cpu->libre = 1;
	cpu->pid = socketCPU_fd;
	list_add(listaCPUs, cpu);
}
