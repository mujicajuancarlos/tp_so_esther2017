#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>
#include<netdb.h>

#define PUERTO 5000 //
#define BACKLOG 2 //cantidad de conex en cola

int main(void){

int newSocket, newSocket2;
struct sockaddr_in cliente;
struct sockaddr_in servidor;
int tamanio_struct;

//struct sockaddr;

servidor.sin_family = AF_INET;
servidor.sin_port = 0; //aca puedo sacar esto y poner htons(PUERTO) 
servidor.sin_addr= inet_addr("127.0.0.1") //aca puedo sacar esto y poner INADDR_ANY;
bzero(&(cliente.sin_zero),8);

/CREACION DE SOCKET CON COMPROBACION DE ERRORES..............

if((newSocket = socket(AF_INET,SOCK_STREAM,0)) == -1){
printf("Error en socket");
exit(-1);
}

//ASOCIACION DE PUERTO CON COMPROBACION DE ERRORES----------------

if(bind(newSocket,(struct sockaddr *)&servidor,sizeof(struct sockaddr)) == -1){
printf("Error en bind");
exit(-1);
}

//ACEPTA Y DA UN NUEVO SOCKKET PARA CONECTAR CON CLIENTE------------------

while(1) {
tamanio_struct = sizeof(struct sockaddr_in);
if((newSocket2 = accept(newSocket,(struct sockaddr *)&cliente,&tamanio_struct)) == -1){
printf("Error en accept");
exit(-1);
}

printf("Se obtuvo una conexion desde: %s", inet_ntoa(cliente.sin_addr));
send(newSocket2,"Bienvenido al servidor.",22,0); //Jugar con variables para el mensaje y la longitud en caso de no ser fijo.
close(newSocket2);
}
}
