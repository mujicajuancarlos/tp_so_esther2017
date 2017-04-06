#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>a
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 38432
#define MAXIMODATOS 30


int main(void){


int newSocket, bytesRecibidos;//ficheros
char buffer[MAXIMODATOS]; //ACA SE GUARDA LO Q  SE RECIBE
char hostname[40];
struct hostent *he;
struct sockaddr_in servidor;
char mensaje[30];



gethostname(hostname,sizeof hostname);

if((newSocket=socket(AF_INET,SOCK_STREAM,0)) == -1){
	printf("Error en socket");
exit(-1);
}

he= gethostbyname(hostname);

if(he == NULL){
	printf("error host");
	exit(-1);
}


servidor.sin_family = AF_INET;
servidor.sin_port = htons(PORT);
//servidor.sin_addr = *((struct in_addr *)he->h_addr);
bcopy((char *)he->h_addr,(char *)&servidor.sin_addr.s_addr, he->h_length);
bzero(&(servidor.sin_zero),8);


if(connect(newSocket,(struct sockaddr *)&servidor, sizeof(servidor)) == -1){
	printf("Error en connect");
	exit(-1);

}

char *a="Hola servidor";
int s;

if((s= send(newSocket,a, strlen(s)+1,0))==-1){
	printf("Error en envio");
	exit(-1);
}



if((bytesRecibidos=recv(newSocket,buffer,MAXIMODATOS,0)) == -1){
	printf("Error en recv");
	exit(-1);

}

buffer[bytesRecibidos] = '\0' ;
		//PONER BARRA CERO Y TERMINAR CADENA

printf("Mensaje recibido desde el servidor: %s", buffer);

close(newSocket);
return EXIT_SUCCESS;
}

