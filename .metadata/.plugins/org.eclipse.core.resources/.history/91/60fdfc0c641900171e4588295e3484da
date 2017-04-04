#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 3550
#define MAXIMODATOS 30


int main(int argc , char *argv[]){
	//aca falta agregar los argumenos para comprobar errores desde linea de comando

int newSocket, bytesRecibidos;//ficheros
char buffer[MAXIMODATOS]; //ACA SE GUARDA LOQ  SE RECIBE

struct hostent *he;
struct sockaddr_in servidor;

//ACA COMPROBACION DE ERRORES DE LINEA DE COMANDO
if(argc !=2){
	printf("Se usa: %s",argv[0]);
			exit(-1);
}

if((he=gethostbyname(argv[1]))==NULL){
	printf("gethostbyname error");
	exit(-1);
}

if((newSocket=socket(AF_INET,SOCK_STREAM,0)) == -1){
	printf("Error en socket");
exit(-1);
}

servidor.sin_family = AF_INET;
servidor.sin_port = 0;
servidor.sin_addr = *((struct in_addr *)he->h_addr);
bzero(&(servidor.sin_zero),8);
//inet_ntoa(*((struct in_addr *)he->h_addr))
if(connect(newSocket,(struct sockaddr *)&servidor, sizeof(struct sockaddr)) == -1){
	printf("Error en connect");
	exit(-1);

}

if((bytesRecibidos=recv(newSocket,buffer,MAXIMODATOS,0)) == -1){
	printf("Error en recv");
	exit(-1);

}

buffer[bytesRecibidos] = //PONER BARRA CERO Y TERMINAR CADENA

printf("Mensaje recibido desde el servidor: %s", buffer);

close(newSocket);
}
