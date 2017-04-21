// includes sockets
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

//includes ArchivoConfiguracion
#include "CPU.h"
#include <commons/config.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define PATH_ARCHIVO_CONFIG "/home/utnso/tp-2017-1c-Los-5-Fant-sticos/CPU/archivoConfiguracion"
#define BUFER_MAX_LEN 2048
#define FILE_MAX 1024
#define PUERTO 1300 // puerto de conexion
#define MAXCONEXIONES 10
#define MAXBUFFER 1024 // tamaño maximo en caracteres que le daremos al buffer

archivoConfiguracion *strConfig;//puntero de mi struct

int obtenerArchivoConfiguracion();
void checkError(int,char*);

int main(void) {
	char buffer[MAXBUFFER];
	int fd_KernelACPU; // el socket que se va a conectar, desde el kernel al CPU
	struct sockaddr_in datosKernel; // datos del kernel representados en una estructura sockaddr_in
	int sizeMensaje;
	int conecta;

	  obtenerArchivoConfiguracion();

	  	puts("\n");

	  	printf("Configuracion desde %s\n",PATH_ARCHIVO_CONFIG);

	  	puts("\n");

	  	printf ("PUERTO:  %d \n", strConfig->PUERTO_CPU);
	    printf("IP_KERNEL:  %s \n", strConfig->IP_CPU);


	// se crea el socket
	fd_KernelACPU = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	checkError(fd_KernelACPU,"Error en socket()\n");

	// cargamos la estructura con la info del server
	datosKernel.sin_family = AF_INET;
	datosKernel.sin_addr.s_addr = INADDR_ANY;
	datosKernel.sin_port = htons (strConfig->PUERTO_CPU);
	memset(datosKernel.sin_zero, 0, 8);

	// me conecto al servidor
	conecta = connect (fd_KernelACPU, (struct sockaddr *) &datosKernel, sizeof (struct sockaddr));

	checkError(conecta,"Error en connect()\n");

	printf ("Iniciando CPU\n\n");
	printf ("Iniciada la conexion al Kernel\n");

	while (1) {
		sizeMensaje = recv (fd_KernelACPU, buffer, MAXBUFFER, 0);

		checkError(sizeMensaje,"Error en recv()\n");

		buffer[sizeMensaje] = '\0'; // agrego caracter de terminacion de string
		printf ("Kernel dice: \"%s\"\n\n", buffer);
		}



	close (fd_KernelACPU);
	printf("Desconectado\n");

	return 0;
}//Cierra

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

    entero = config_get_int_value(archivoConfiguracion,"PUERTO_CPU");//guarda el dato en la var entero
    strConfig->PUERTO_CPU = entero;//asigna el numero guardado en entero, en la estructura

    aux = config_get_string_value(archivoConfiguracion, "IP_CPU");
    strcpy(strConfig->IP_CPU, aux);



    config_destroy(archivoConfiguracion);

        return 0 ;

}

void checkError(int valor, char* mensaje){
	if(valor<0){
		printf("%s \n",mensaje);
		exit(-1);
	}
}
