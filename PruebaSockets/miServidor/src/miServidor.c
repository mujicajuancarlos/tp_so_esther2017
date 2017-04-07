/*
 ============================================================================
 Name        : miServidor.c
 Author      : Nicolas Wallovits
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// includes libreria + i/o estandar
#include <stdio.h>
#include <stdlib.h>

// includes sockets
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#define PUERTO 1299 // puerto de conexion
#define MAXCONEXIONES 3 // maximas conexiones posibles en cola (en este caso no hace diferencia)
#define MAXBUFFER 1024 // tamaño maximo en caracteres que le daremos al buffer

int main(void) {
	int hembra; // socket que recibe la conexion
	int macho; // socket que se conecta
	int addrlen = sizeof (struct sockaddr_in); // voy a necesitar de tamaño de direccion el tamaño que ocupe un sockaddr_in
	struct sockaddr_in datosServer, datosCliente; /* datos del cliente y servidor representados en una
													estructura sockaddr_in */
	char buffer[MAXBUFFER]; // el buffer utilizado para recibir mensajes será un array del tamaño maximo definido
	int clienteConectado; // un indicador que nos dice si en un momento dado el cliente se encuentra conectado
	int sizeMensaje; // tamaño del mensaje recibido
	int on=1; // necesario para setsockop

	// creamos el socket (el que recibe la conexion)
	hembra = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// asignamos los datos de nuestro server
	datosServer.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosServer.sin_addr.s_addr = INADDR_ANY; // LocalHost
	datosServer.sin_port = htons (PUERTO); // puerto a conectarse
	memset (datosServer.sin_zero, 0, 8); // campo raro ?

	/* si el server se cierra bruscamente, el puerto por el que conectaba queda ocupado, lo cual es molesto.
	 esto se puede solucionar con la siguiente linea */
	setsockopt (hembra, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	// ya llené la estructura con ip:puerto (datosServer), ahora enlazo el socket
	bind (hembra, (struct sockaddr*) &datosServer, sizeof (datosServer));

	// finalmente ponemos el socket a escuchar
	listen (hembra, MAXCONEXIONES);

	printf ("Nuestro servidor esta conectado con IP %s\n", inet_ntoa((struct in_addr)datosServer.sin_addr));

	while (1) { // bucle infinito
		printf ("Esperando por conexiones \n");

		//se acepta una conexion
		macho = accept (hembra, &datosCliente, &addrlen);
		clienteConectado = 1;

		// ahora puedo acceder a los datos del cliente, entonces puedo mostrar la IP del cliente conectado
		printf ("Se conecto un nuevo cliente con IP: %s\n", inet_ntoa((struct in_addr)datosCliente.sin_addr));

		while (clienteConectado) { // clienteConectado es nuestro indicador
			printf ("Aguardando mensaje\n");

			// recibo mensaje del cliente
			sizeMensaje = recv (macho, buffer, MAXBUFFER, 0);

			if (sizeMensaje > 0) { // si recibi mensaje lo imprimo
				buffer[sizeMensaje] = '\0'; // agrego caracter de terminacion de string
				printf ("Mensaje recibido: \"%s\"\n", buffer);
			}
			else clienteConectado = 0;
		}
		printf ("Se desconecto el cliente \n");
	}

	// finalmente se cierra el socket del cliente
	close (macho);

	return 0;
}
