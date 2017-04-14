#include "socket.h"
//#define PUERTO 5486
#define BACKLOG 100

int crearSocketServer(int puerto) {

	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
		//direccionServidor.sin_addr.s_addr = htonl(ip);
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
		//direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
		//direccionServidor.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		//memset(&(direccionServidor.sin_zero, '\0', 8);
	direccionServidor.sin_port = htons(puerto);

		//creo el file descriptor
	int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFileDescriptor == -1) {
		error_show(
				"Create socket file descriptor failed FD: %d address: %c port: %d",
				socketFileDescriptor, INADDR_LOOPBACK, puerto);
		return SOCKET_FAILURE;
	}

	int activado = 1;

	if (setsockopt(socketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &activado,
			sizeof(activado)) != 0) {
		error_show("Set socket option failed FD: %d address: %c port: %d",
				socketFileDescriptor, INADDR_LOOPBACK, puerto);
		return SOCKET_FAILURE;
	}

	if (bind(socketFileDescriptor, (void*) &direccionServidor,
			sizeof(direccionServidor)) != 0) {
		if (bind(socketFileDescriptor, (void*) &direccionServidor,
			sizeof(direccionServidor)) == -1) {
			perror("Socket bind failed");
			exit(1);
		}
		error_show("Socket bind failed FD: %d address: %c port: %d",
				socketFileDescriptor, INADDR_LOOPBACK, puerto);
		return SOCKET_FAILURE;
	}

	if (listen(socketFileDescriptor, BACKLOG) == -1) {

		error_show("Socket listen failed FD: %d address: %c port: %d",
				socketFileDescriptor, INADDR_LOOPBACK, puerto);
		
		close(socketFileDescriptor);
		return SOCKET_FAILURE;
	}

	return socketFileDescriptor;
}

int aceptarConexionCliente(int socketServerFileDescriptor) {

	struct sockaddr procesoKernel;
	socklen_t longitudProcesoKernel = sizeof(procesoKernel);
	int newSocketFileDescriptor;

	newSocketFileDescriptor = accept(socketServerFileDescriptor, &procesoKernel,
			&longitudProcesoKernel);

	if (newSocketFileDescriptor == -1) {
		error_show("Socket accept failed FD: %d", socketServerFileDescriptor);
		return SOCKET_FAILURE;
	}

	return newSocketFileDescriptor;
}
