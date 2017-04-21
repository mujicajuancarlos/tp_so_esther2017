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
#include <sys/stat.h>

//includes archivoConfiguracion
#include "Kernel.h"
#include <commons/config.h>


#define PATH_ARCHIVO_CONFIG "/home/utnso/tp-2017-1c-Los-5-Fant-sticos/Kernel/archivoConfiguracion"
#define BUFER_MAX_LEN 2048
#define FILE_MAX 1024
#define PUERTOCONSOLA 1299 // puerto de conexion
#define PUERTOMEMORIA 1302
#define PUERTOFS 5000
#define PUERTOCPU 1300
#define MAXCONEXIONES 10 // maximas conexiones posibles en cola (en este caso no hace diferencia)
#define MAXCONEXIONES_SALIDA 10
#define MAXBUFFER 1024 // tamaño maximo en caracteres que le daremos al buffer

archivoConfiguracion *strConfig;//puntero de mi struct

int obtenerArchivoConfiguracion();

int main(void) {
	int hembra; // socket de entrada
	int macho;
	int fd_FS;// socket de salida
	int fd_Memoria;
	int maxDescriptors = 4;
	int addrlen = sizeof (struct sockaddr_in);
	struct sockaddr_in datosEntrada, datosFS, datosCliente, datosMemoria, datosSalida; // datos de entrada, salida y cliente
	char buffer[MAXBUFFER]; // el buffer utilizado para recibir mensajes será un array del tamaño maximo definido
	int on=1; // necesario para setsockop
	int conecta;
	fd_set readDescriptors; // set de fd que me interesa leer
	fd_set writeDescriptors; // set de fd a los que me interesa escribir
	fd_set readDescriptorsOriginales;

	obtenerArchivoConfiguracion();
		puts("\n");

		printf("Configuracion desde %s\n",PATH_ARCHIVO_CONFIG);

		puts("\n");

		printf ("PUERTO_PROG:  %d \n", strConfig->PUERTO_PROG );
	    printf ("PUERTO_CPU:  %d \n", strConfig->PUERTO_CPU );
	    printf("IP_MEMORIA:  %s \n",strConfig->IP_MEMORIA);
	    printf ("PUERTO_MEMORIA: %d \n", strConfig->PUERTO_MEMORIA);
	    printf("IP_FS:  %s \n",strConfig->IP_FS);
	    printf ("PUERTO_FS:  %d \n", strConfig->PUERTO_FS );
	    printf ("QUANTUM:  %d \n", strConfig->QUANTUM );
	    printf ("QUANTUM_SLEEP: %d \n", strConfig->QUANTUM_SLEEP );
	    printf("ALGORITMO: %s \n",strConfig->ALGORITMO);
	    printf ("GRADO_MULTIPROG: %d \n", strConfig->GRADO_MULTIPROG);
	    printf ("STACK_SIZE: %d \n", strConfig->STACK_SIZE);



	// creamos los socket de entrada y salida
	hembra = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Escucha CPUs

	fd_FS = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Para conectar con FS

	fd_Memoria = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Para conectar Memoria

	macho = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Escucha Consolas

	checkError(hembra,"Error en socket(hembra)\n");

	checkError(fd_FS,"Error en socket(macho)\n");

	checkError(fd_Memoria,"Error en socket(macho)\n");

	checkError(macho,"Error en socket(macho)\n");

	// asignamos los datos de entrada (CPU y Consola)
	datosEntrada.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosEntrada.sin_addr.s_addr = INADDR_ANY; // LocalHost
	datosEntrada.sin_port = htons (strConfig->PUERTO_PROG); // puerto a conectarse
	memset (datosEntrada.sin_zero, 0, 8); // campo raro ?

	datosSalida.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosSalida.sin_addr.s_addr = INADDR_ANY; // LocalHost
	datosSalida.sin_port = htons (strConfig->PUERTO_CPU); // puerto a conectarse
	memset (datosSalida.sin_zero, 0, 8); // campo raro ?

	// asignamos los datos de salida
	datosFS.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosFS.sin_addr.s_addr = inet_addr(strConfig->IP_FS); // LocalHost
	datosFS.sin_port = htons (strConfig->PUERTO_FS); // puerto a conectarse
	memset (datosFS.sin_zero, 0, 8); // campo raro ?

	datosMemoria.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosMemoria.sin_addr.s_addr = inet_addr(strConfig->IP_MEMORIA); // LocalHost
	datosMemoria.sin_port = htons (strConfig->PUERTO_MEMORIA); // puerto a conectarse
	memset (datosMemoria.sin_zero, 0, 8); // campo raro ?

	/* si el server se cierra bruscamente, el puerto por el que conectaba queda ocupado, lo cual es molesto.
	 esto se puede solucionar con la siguiente linea */
	setsockopt (hembra, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	setsockopt (macho, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	conecta= connect (fd_Memoria, (struct sockaddr *) &datosMemoria, sizeof (struct sockaddr));

	checkError(conecta,"Error al conectar con Memoria\n");

	printf("Conectado con memoria\n");

	conecta = connect(fd_FS, (struct sockaddr *) &datosFS, sizeof(struct sockaddr));

	checkError(conecta,"Error al conectar con File System\n");

	printf("Conectado con File System\n");



	// enlazo sockets a las estructuras de entrada y salida respectivamente
	bind (hembra, (struct sockaddr*) &datosEntrada, sizeof (datosEntrada));
	bind (macho, (struct sockaddr*) &datosSalida, sizeof (datosSalida));

	// finalmente los ponemos a escuchar
	listen (hembra, MAXCONEXIONES);
	listen (macho, MAXCONEXIONES);


	printf ("Nuestro kernel esta conectado con IP %s\n\n", inet_ntoa((struct in_addr)datosEntrada.sin_addr));

	// inicializo los fd de lectura y agrego los propios del kernel

	FD_ZERO (&readDescriptorsOriginales);
	FD_SET (hembra, &readDescriptorsOriginales);
	FD_SET(macho, &readDescriptorsOriginales);
	FD_ZERO (&writeDescriptors);
	FD_SET(fd_FS, &writeDescriptors);
	FD_SET(fd_Memoria, &writeDescriptors);

	printf ("Esperando conexiones de consolas y CPUs \n\n");

	int sizeMensaje;

	while(1){

		readDescriptors = readDescriptorsOriginales;

		if (select (maxDescriptors + 1, &readDescriptors, NULL, NULL, NULL) == -1)
			printf ("Error en select");

		int i;

		for (i = 0; i <= maxDescriptors; i++) {
			if (FD_ISSET (i, &readDescriptors)) {
				// si la actividad es del macho me conecto a una salida
				if (i == macho) {
					int saliente;
					saliente = accept (macho, &datosCliente, &addrlen);
					send (saliente, "Te conectaste", 13, 0);
					FD_SET (saliente, &writeDescriptors);
					FD_SET (saliente, &readDescriptorsOriginales); // lo agregamos tambien a lectura para darnos cuenta si se cerro el proceso
					printf ("Se me conecto un nuevo proceso de salida\nSe le asigna numero de proceso = %i\n\n", (saliente - 4));
					if (saliente > maxDescriptors)
						maxDescriptors = saliente;
				}
				else if (i == hembra) {
					// si la actividad es de hembra recibi conexion nueva
					int entrante; // socket que se conecta
					entrante = accept (hembra, &datosCliente, &addrlen);
					printf ("Se conecto una nueva consola. \nSe le asigna numero de proceso = %i \nIP de consola: %s\n\n", (entrante - 4), inet_ntoa((struct in_addr)datosCliente.sin_addr));
					FD_SET (entrante, &readDescriptorsOriginales);
					if (entrante > maxDescriptors)
						maxDescriptors = entrante;
				}
				else if (FD_ISSET (i, &writeDescriptors)) { // si tambien pertenece a los writeDescriptors se cerro un proceso de salida
					close (i);
					FD_CLR (i, &readDescriptorsOriginales);
					FD_CLR (i, &writeDescriptors);
					printf ("Proceso de salida con numero de proceso %i desconectado\n\n", (i - 4));
				}
				else {
					// sino la actividad es de algun cliente
					 sizeMensaje = recv (i, buffer, MAXBUFFER, 0);
					if (sizeMensaje > 0) {
						// si recibi mensaje lo imprimo
						buffer[sizeMensaje] = '\0'; // agrego caracter de terminacion de string
						printf ("Consola numero de proceso %i dice: \"%s\"\n\n", (i - 4), buffer);
						int j = 0;
						for (j = 0; j <= maxDescriptors; j++) {
							if (FD_ISSET (j,&writeDescriptors))
								send (j, buffer, strlen(buffer), 0); // se envia el mensaje
						}

					}
					else {
						close (i);
						FD_CLR (i, &readDescriptorsOriginales);
						printf ("Consola numero de proceso %i desconectada \n\n", (i - 4));
					}
				}
			}
		}
	}


	close (macho);
	close (hembra);
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

    aux=(char*)malloc (BUFER_MAX_LEN);//EN ESTA VAR VOY OBTENIENDO LOS VALORES DEL ARCHIVO
    memset(aux,'\0', BUFER_MAX_LEN);

    if ( archivoConfiguracion == NULL )
    {
        return -1;
    }

    entero = config_get_int_value(archivoConfiguracion,"PUERTO_PROG");//guarda el dato en la var entero
    strConfig->PUERTO_PROG= entero;//asigna el numero guardado en entero, en la estructura

    entero = config_get_int_value(archivoConfiguracion,"PUERTO_CPU");//guarda el dato en la var entero
    strConfig->PUERTO_CPU= entero;//asigna el numero guardado en entero, en la estructura

    aux = config_get_string_value(archivoConfiguracion, "IP_MEMORIA");
    strcpy(strConfig->IP_MEMORIA, aux);

    entero = config_get_int_value(archivoConfiguracion,"PUERTO_MEMORIA");//guarda el dato en la var entero
    strConfig->PUERTO_MEMORIA= entero;//asigna el numero guardado en entero, en la estructura

    aux = config_get_string_value(archivoConfiguracion, "IP_FS");
    strcpy(strConfig->IP_FS, aux);

    entero = config_get_int_value(archivoConfiguracion,"PUERTO_FS");//guarda el dato en la var entero
    strConfig->PUERTO_FS= entero;//asigna el numero guardado en entero, en la estructura

    entero = config_get_int_value(archivoConfiguracion,"QUANTUM");//guarda el dato en la var entero
    strConfig->QUANTUM= entero;//asigna el numero guardado en entero, en la estructura

    entero = config_get_int_value(archivoConfiguracion,"QUANTUM_SLEEP");//guarda el dato en la var entero
    strConfig->QUANTUM_SLEEP= entero;//asigna el numero guardado en entero, en la estructura

    aux = config_get_string_value(archivoConfiguracion, "ALGORITMO");
    strcpy(strConfig->ALGORITMO, aux);

    entero = config_get_int_value(archivoConfiguracion,"GRADO_MULTIPROG");//guarda el dato en la var entero
    strConfig->GRADO_MULTIPROG= entero;//asigna el numero guardado en entero, en la estructura

    entero = config_get_int_value(archivoConfiguracion,"STACK_SIZE");//guarda el dato en la var entero
    strConfig->STACK_SIZE= entero;//asigna el numero guardado en entero, en la estructura


    config_destroy(archivoConfiguracion);

        return 0 ;

}

void checkError(int valor, char* mensaje){
	if(valor<0){
		printf("%s \n",mensaje);
		exit(-1);
	}
}

