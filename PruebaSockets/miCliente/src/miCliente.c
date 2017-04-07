/*
 ============================================================================
 Name        : miCliente.c
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
#define MAXBUFFER 1024 // tamaño maximo en caracteres que le daremos al buffer

int main(void) {
	char buffer[MAXBUFFER];
	int macho; // el socket que se va a conectar, desde el cliente al servidor
	struct sockaddr_in datosServer; // datos del servidor representados en una estructura sockaddr_in
	int enviados; // > 0 si se envio algo

	// se crea el socket
	macho = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// cargamos la estructura con la info del server
	datosServer.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosServer.sin_addr.s_addr = INADDR_ANY; // localHost
	datosServer.sin_port = htons (PUERTO); // puerto a conectarse
	memset(datosServer.sin_zero, 0, 8); // campo raro ?

	// me conecto al servidor
	connect (macho, (struct sockaddr *) &datosServer, sizeof (struct sockaddr));

	printf ("Conectado al servidor \nPara desconectarse escriba \"salir\"\n");

	while (1) {
		printf ("Escriba el mensaje a enviar> ");
		fgets (buffer, MAXBUFFER, stdin); // tomamos el input del usuario en pantalla
		buffer [strlen(buffer)-1] = '\0'; // strlen devuelve el tamaño del string sin contar el caracter nulo

		if (strcmp (buffer,"salir")==0) break; // si el string que tipeo el usuario fue "salir" entonces se sale

		enviados = send (macho, buffer, strlen(buffer), 0); // se envia el mensaje

		if (enviados >0) // si se envio algo entonces se da el mensaje de enviado
			printf ("Enviado\n");
		else {
			printf ("Error servidor\n"); // si no se pudo enviar nada tiramos un error
			break;
		}
	}

	close (macho);
	printf("Desconectado\n");

	return 0;
}
