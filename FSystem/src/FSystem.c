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
#include <errno.h>

//includes archivoConfiguracion
#include "FSystem.h"
#include <commons/config.h>

#define PATH_ARCHIVO_CONFIG "/home/utnso/tp-2017-1c-Los-5-Fant-sticos/FSystem/archivoConfiguracion"
#define BUFER_MAX_LEN 2048
#define FILE_MAX 1024
#define PORT 5000
#define MAXCONEXIONES 10
#define MAXBUFFER 1024 // tamaño maximo en caracteres que le daremos al buffer

archivoConfiguracion *strConfig;//puntero de mi struct

int obtenerArchivoConfiguracion();

int main(void) {
	char buffer[MAXBUFFER];
	int fd_KernelAFS; // el socket que se va a conectar, desde el kernel al CPU
	struct sockaddr_in datosKernel; // datos del kernel representados en una estructura sockaddr_in
	int sizeMensaje;
	int addrlen = sizeof (struct sockaddr_in);
	int aceptado;
	int binder;
	int listening;
	int sender;

	obtenerArchivoConfiguracion();
		puts("\n");

		printf("Configuracion desde %s\n",PATH_ARCHIVO_CONFIG);

		puts("\n");

		printf ("PUERTO:  %d \n", strConfig->PUERTO);

		printf("PUNTO_MONTAJE: %d \n",strConfig->PUNTO_MONTAJE);


	// se crea el socket
	fd_KernelAFS = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	checkError(fd_KernelAFS,"Error en socket()\n");

	// cargamos la estructura con la info del server
	datosKernel.sin_family = AF_INET;
	datosKernel.sin_addr.s_addr = INADDR_ANY;
	datosKernel.sin_port = htons (strConfig->PUERTO);
	memset(datosKernel.sin_zero, 0, 8);


	int on = 1;

	setsockopt (fd_KernelAFS, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	// me conecto al servidor

	binder = bind (fd_KernelAFS, (struct sockaddr*) &datosKernel, sizeof (datosKernel));

	checkError(binder,"Error en bind()\n");

	listening = listen(fd_KernelAFS,1);

	checkError(listening,"Error en listen()\n");

	printf ("Iniciando File System\n\n");

	aceptado = accept(fd_KernelAFS, &datosKernel, &addrlen);

	checkError(aceptado,"Error al conectar con Kernel");

	printf ("Kernel conectado\n\n");


	/*
	while(1){

		printf ("Escriba el mensaje a enviar>\n\n ");

		fgets (buffer, MAXBUFFER, stdin); // tomamos el input del usuario en pantalla

		buffer [strlen(buffer)-1] = '\0'; // strlen devuelve el tamaño del string sin contar el caracter nulo

		if (strcmp (buffer,"salir")==0) break; // si el string que tipeo el usuario fue "salir" entonces se sale

		sender = send (aceptado, buffer, strlen(buffer), 0); // se envia el mensaje

		checkError(sender,"Error en send()\n");

		printf ("Enviado\n");

	}
	*/



///*
 	 while (1) {

		sizeMensaje = recv (aceptado, buffer, MAXBUFFER, 0);

		checkError(sizeMensaje,"Error en recv()\n");

		if(sizeMensaje==0){

			printf("Kernel desconectado\n");
		}

		buffer[sizeMensaje] = '\0'; // agrego caracter de terminacion de string

		printf ("Kernel dice: \"%s\"\n\n", buffer);

		memset(buffer,0,sizeof(buffer));


	}
	//*/

	close (fd_KernelAFS);
	printf("Desconectado\n");

	return 0;
}


int obtenerArchivoConfiguracion()
{
    t_config *archivoConfiguracion = malloc(sizeof(t_config));


    strConfig = malloc(sizeof(archivoConfiguracion));
    char path_file[FILE_MAX];
    memset(path_file, '\0', FILE_MAX);
    strcpy(path_file, PATH_ARCHIVO_CONFIG);//Copio el contenido de la ruta paara pasarle por parametro a config_create (commons)

    char* aux;
    int entero;

    archivoConfiguracion = config_create(path_file);

    aux =(char*)malloc (BUFER_MAX_LEN);//EN ESTA VAR VOY OBTENIENDO LOS VALORES DEL ARCHIVO
    memset(aux,'\0', BUFER_MAX_LEN);

    if ( archivoConfiguracion == NULL )
    {
        return -1;
    }

    entero = config_get_int_value(archivoConfiguracion,"PUERTO");//guarda el dato en la var entero
    strConfig->PUERTO = entero;//asigna el numero guardado en entero, en la estructura

    aux = config_get_string_value(archivoConfiguracion, "PUNTO_MONTAJE");
    strcpy(strConfig->PUNTO_MONTAJE, aux);



    config_destroy(archivoConfiguracion);

        return 0 ;

}


void checkError(int valor, char* mensaje){
	if(valor<0){
		printf("%s\n",mensaje);
		exit(-1);
	}
}
