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
#include <sys/stat.h>
#include <sys/types.h>

//include archivoConfiguracion
#include "Consola.h"
#include <commons/config.h>

#define PATH_ARCHIVO_CONFIG "/home/utnso/tp-2017-1c-Los-5-Fant-sticos/Consola/archivoConfiguracion"
#define PUERTO 1299 // puerto de conexion
#define MAXBUFFER 1024 // tamaño maximo en caracteres que le daremos al buffer
#define BUFER_MAX_LEN 2048
#define FILE_MAX 1024

archivoConfiguracion *strConfig;//puntero de mi struct

void checkError(int , char*);
int obtenerArchivoConfiguracion();

int main(void) {
	char buffer[MAXBUFFER];
	int macho; // el socket que se va a conectar, desde el cliente al servidor
	struct sockaddr_in datosServer; // datos del servidor representados en una estructura sockaddr_in
	int enviados; // > 0 si se envio algo
	int conecta;
	obtenerArchivoConfiguracion();
		puts("\n");

		printf("Configuracion desde %s\n",PATH_ARCHIVO_CONFIG);

  		puts("\n");

		printf ("PUERTO_KERNEL: %d \n", strConfig->PUERTO_KERNEL );
	    printf("IP_KERNEL %s \n",strConfig->IP_KERNEL);


	// se crea el socket
	macho = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

	checkError(macho,"Error en socket()\n");

	// cargamos la estructura con la info del server

	datosServer.sin_family = AF_INET; // tipo de direccion, en este caso Internet Protocol v4
	datosServer.sin_addr.s_addr = INADDR_ANY; // localHost
	datosServer.sin_port = htons (strConfig->PUERTO_KERNEL); // puerto a conectarse
	memset(datosServer.sin_zero, 0, 8); // campo raro ?

	// me conecto al servidor
	conecta= connect (macho, (struct sockaddr *) &datosServer, sizeof (struct sockaddr));

	checkError(conecta,"Error en connect()\n");

	printf ("Conectado al servidor \nPara desconectarse escriba \"salir\"\n");

	while (1) {
		printf ("Escriba el mensaje a enviar> ");

		fgets (buffer, MAXBUFFER, stdin); // tomamos el input del usuario en pantalla

		buffer [strlen(buffer)-1] = '\0'; // strlen devuelve el tamaño del string sin contar el caracter nulo

		if (strcmp (buffer,"salir")==0) break; // si el string que tipeo el usuario fue "salir" entonces se sale

		enviados = send (macho, buffer, strlen(buffer), 0); // se envia el mensaje

		checkError(enviados,"Error en send()\n");

		printf ("Enviado\n");

	}

	close (macho);
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

    entero = config_get_int_value(archivoConfiguracion,"PUERTO_KERNEL");//guarda el dato en la var entero
    strConfig->PUERTO_KERNEL = entero;//asigna el numero guardado en entero, en la estructura

    aux = config_get_string_value(archivoConfiguracion, "IP_KERNEL");
    strcpy(strConfig->IP_KERNEL, aux);



    config_destroy(archivoConfiguracion);

        return 0 ;

}

void checkError(int valor, char* mensaje){
	if(valor<0){
		printf("%s \n",mensaje);
		exit(-1);
	}
}
