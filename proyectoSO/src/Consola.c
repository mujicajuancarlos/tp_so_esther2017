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


#define PUERTO 5000
#define MAXIMODATOS 30

int main(int argc, char *argv[]){

int newSocket, bytesRecibidos;//ficheros

char buffer[MAXIMODATOS];//aca se guarda lo q se recibe

struct hostent *he //guarda datos de la maquina

struct sockaddr_in servidor;

if(argc !=2){ //porq el parametro es la ip solamente
printf("Error"); //aca es donde imprimia un directorio en vez de la ip
exit(-1);
}

if((he= gethostbyname(argv[1])) == NULL){
printf("error en geth");
exit(-1);
}

if((newSocket=socket(AF_INET,SOCK_STREAM,0)) == -1){

printf("Error en socket");
exit(-1);
}

servidor.sin_family = AF_INET;
servidor.sin_port = 0;//puede ir htons(PUERTO)
servidor.sin_addr = *((struct in_addr *)he->h_addr);
bzero(&(servidor.sin_zero),8);

if(connect(newSocket,(struct sockaddr *)&servidor, sizeof(struct sockaddr)) == -1{
printf("Error en conexion");
exit(-1);
}

if((bytesRecibidos = recv(newSocket,buffer,MAXIMODATOS,0)) == -1) {
printf("Error en recv");
exit(-1);
}

buffer[bytesRecibidos] = '\0'
printf("Mensaje recibido desde el servidor: %s",buffer);
close(newSocket);
}
   
